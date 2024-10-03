#include <multitasking.h>

using namespace bljOS;
using namespace bljOS::common;

void printf(uint8_t* str, int32_t x, int32_t y, uint32_t color);

SpinlockCnt TASK_LL_MODIFY = {0};

void Task::taskAttachDefTermios(Task *task){
  memset(&task->term, 0, sizeof(termios));
  task->term.c_iflag = BRKINT | ICRNL | INPCK | ISTRIP | IXON;
  task->term.c_oflag = OPOST;
  task->term.c_cflag = CS8 | CREAD | CLOCAL;
  task->term.c_lflag = ECHO | ICANON | IEXTEN | ISIG;
  task->term.c_line = 0;
  task->term.c_cc[VINTR] = 3;     // Ctrl-C
  task->term.c_cc[VQUIT] = 28;    // Ctrl-task->term.c_cc[VERASE] = 127; // DEL
  task->term.c_cc[VKILL] = 21;    // Ctrl-U
  task->term.c_cc[VEOF] = 4;      // Ctrl-D
  task->term.c_cc[VTIME] = 0;     // No timer
  task->term.c_cc[VMIN] = 1;      // Return each byte
  task->term.c_cc[VSTART] = 17;   // Ctrl-Q
  task->term.c_cc[VSTOP] = 19;    // Ctrl-S
  task->term.c_cc[VSUSP] = 26;    // Ctrl-Z
  task->term.c_cc[VREPRINT] = 18; // Ctrl-R
  task->term.c_cc[VDISCARD] = 15; // Ctrl-O
  task->term.c_cc[VWERASE] = 23;  // Ctrl-W
  task->term.c_cc[VLNEXT] = 22;   // Ctrl-V
  // Initialize other control characters to 0
  for (int i = 16; i < NCCS; i++) {
    task->term.c_cc[i] = 0;
  }
}

Task::Task(bljOS::GlobalDescriptorTable* gdt, bljOS::common::uint32_t rip, bool kernel_task, bljOS::common::uint32_t* pagedir, bljOS::common::uint32_t argc, char** argv){
    cpustate->eax = 0;
    cpustate->ebx = 0;
    cpustate->ecx = 0;
    cpustate->edx = 0;

    cpustate->esi = 0;
    cpustate->edi = 0;
    cpustate->ebp = 0;

    cpustate->eip = rip;
    cpustate->cs = gdt->getCodeSegmentSelector();
    cpustate->eflags = 0x202;

    uint32_t code_selector = (uint32_t)kernel_task ? gdt->getCodeSegmentSelector() : 0;
    uint32_t data_selector = (uint32_t)kernel_task ? gdt->getDataSegmentSelector() : 0;

    registers.ds = data_selector;
    registers.cs = code_selector;
    registers.usermode_ss = data_selector;
    registers.usermode_esp = USER_STACK_BOTTOM;

    registers.eflags = 0x202;
    registers.eip = rip;

    this->id = id;
    this->kernel_task = kernel_task;
    this->state = TASK_STATE_CREATED;
    this->pagedir = pagedir;

    this->heap_start = USER_HEAP_START;
    this->heap_end = USER_HEAP_START;

    this->mmap_start = USER_MMAP_START;
    this->mmap_end = USER_MMAP_START;

    this->umask = 0020 | 0002;

    memset(this->fpuenv, 0, 512);
    ((uint16_t*)this->fpuenv)[0] = 0x37f;
    this->mxcsr = 0x1f80;

    taskAttachDefTermios(this);
}

Task::~Task(){

}

TaskManager::TaskManager(){

}

TaskManager::~TaskManager(){

}

Task* TaskManager::addTask(bljOS::GlobalDescriptorTable* gdt, bljOS::common::uint32_t rip, bool kernel_task, bljOS::common::uint32_t* pagedir, bljOS::common::uint32_t argc, char** argv){
//     spinlockCntWriteAcquire(&TASK_LL_MODIFY);
    Task* browse = firstTask;

    while(browse){
        if(browse->next)
            break;
        browse = browse->next;
    }

    if(!browse)
        printf((uint8_t*)"error", 20, 20, 0xebdbb2);

//     spinlockCntWriteRelease(&TASK_LL_MODIFY);
    Task* target = (Task*)MemoryManager::activeMemoryManager->malloc(sizeof(Task));
    memset(target, 0, sizeof(Task));
    browse->next = target;
    if(target!=0)
        new (target)Task(gdt, rip, kernel_task, pagedir, argc, argv);

    return target;

}

CPUState* TaskManager::Schedule(CPUState* cpustate){
    if(firstTask == nullptr)
        return cpustate;

    if(currentTask)
        currentTask->cpustate = cpustate;

    return currentTask->cpustate;
}


