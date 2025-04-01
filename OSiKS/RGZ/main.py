from tkinter import *
import time
from tkinter.messagebox import showinfo

class CannotAddTask(Exception):
    pass

class CannotRemoveTask(Exception):
    pass

class Task:
    def __init__(self, name:str, size:int, time_of_completion:int, first_block:int):
        self.name = name
        self.size = size
        self.time_of_completion = time_of_completion
        self.first_block = first_block

class RAM:
    def __init__(self, size):
        self.memory_size = size
        self.memory_array = [0]*self.memory_size
        self.tasks = []
        self.time=0

    def showMemory(self):
        print(self.memory_array)

    def showTasks(self):
        i=1
        print("#\tname\tsize\ttime_of_completion")
        for task in self.tasks:
            print(f"{i}\t{task.name}\t{task.size}\t{task.time_of_completion}")
            i+=1

    def addTask(self, name, size, time_to_complete):
        sequential_free_memory = 0
        free_memory_start = 0
        for i in range(self.memory_size):
            if self.memory_array[i] == 0:
                sequential_free_memory +=1
            if sequential_free_memory == size:
                self.tasks.append(Task(name,size,time_to_complete+self.time,free_memory_start))
                for j in range(free_memory_start, free_memory_start+size):
                    self.memory_array[j]=1
                return
            if self.memory_array[i] == 1:
                if (i+1) > (self.memory_size-1):
                    raise CannotAddTask
                sequential_free_memory = 0
                free_memory_start = i+1
        raise CannotAddTask

    def removeTask(self, task_number):
        if task_number < 1:
            raise CannotRemoveTask
        if task_number > len(self.tasks):
            raise CannotRemoveTask
        for i in range(self.tasks[task_number-1].first_block, self.tasks[task_number-1].first_block+self.tasks[task_number-1].size):
            self.memory_array[i]=0
        self.tasks.pop(task_number-1)

    def incrementTime(self):
        self.time+=1
        i=0
        while i < len(self.tasks):
            if self.tasks[i].time_of_completion == self.time:
                self.removeTask(i+1)
            else:
                i+=1

    # def defragmentMemory(self):
    #     self.memory_array = [0]*self.memory_size
    #     sum_of_sizes = 0
    #     for task in self.tasks:
    #         sum_of_sizes += task.size
    #     if sum_of_sizes > self.memory_size:
    #         raise Exception
    #     for i in range(sum_of_sizes):
    #         self.memory_array[i]=1

    def defragmentMemory(self):
        array_of_names=[]
        array_of_sizes=[]
        array_of_times=[]
        for task in self.tasks:
            array_of_names.append(task.name)
            array_of_sizes.append(task.size)
            array_of_times.append(task.time_of_completion - self.time)
        number_of_tasks = len(self.tasks)
        for i in range(number_of_tasks):
            self.removeTask(1)
        for i in range(number_of_tasks):
            self.addTask(array_of_names[i],array_of_sizes[i],array_of_times[i])

def update():
    current_time=str(ram.time)
    time_label.config(text=f'Current time is: {current_time}')
    tasks_headline.config(text=f'Current tasks (time is {current_time}):')
    #memory array render
    m=''
    for i in range(ram.memory_size):
        if ram.memory_array[i] == 0:
            m+='0'
        else:
            m+='#'
        if i%50==49:
            m+='\n'
    memory.config(text=m)

    #tasks render
    t=''
    i = 1
    t+="№\tSize\tF\tBlock 0\n"
    for task in ram.tasks:
        t+=f"{i}\t{task.size}\t{task.time_of_completion}\t{task.first_block+1}\n"
        i += 1
    tasks.config(text=t, font=8)

def incrementTime():
    ram.incrementTime()
    update()

def setMemory():
    global ram
    try:
        mem_size=int(memory_entry.get())
    except ValueError:
        showinfo("Error", 'Invalid input')
        return
    if mem_size < 0 or mem_size > 1024:
        showinfo("Error", 'Memory size out of valid range (0..1024)')
        return
    ram=RAM(int(memory_entry.get()))
    update()

def addTask():
    try:
        t_size = int(task_size_entry.get())
        t_time = int(task_time_entry.get())
    except ValueError:
        showinfo("Error", 'Invalid input')
        return
    if t_size < 1:
        showinfo("Error", 'Task size must be greater than zero')
        return
    if t_time < 1:
        showinfo("Error", 'Task lifetime must be greater than zero')
        return
    try:
        ram.addTask('placeholder', t_size, t_time)
    except CannotAddTask:
        showinfo("Error", 'Not enough memory for the task')
        return
    update()

def removeTask():
    try:
        task_number = int(task_remove_entry.get())
    except ValueError:
        showinfo("Error", 'Invalid input')
        return
    if task_number < 1:
        showinfo("Error", 'Invalid task number')
        return
    try:
        ram.removeTask(task_number)
    except CannotRemoveTask:
        showinfo("Error", 'There is no task with such a number')
        return
    update()

def defragment():
    ram.defragmentMemory()
    update()

if __name__ == "__main__":
    global ram
    ram=RAM(0)
    window = Tk()
    window.title('Эмулятор распределителя оперативной памяти')
    window.geometry('1200x600')
    #Headlines
    memory_headline = Label(window, text='Memory array:', font=('Arial', 20), fg='blue', justify="left")
    memory_headline.grid(row=10, column=10)

    tasks_headline = Label(window, text='Current tasks:', font=('Arial', 20),fg='blue', justify="right")
    tasks_headline.grid(row=10, column=13, sticky='w')
    #Displays
    memory = Label(window, text='', font=('Arial',8), justify="left")
    memory.grid(row=11, column=10)

    tasks = Label(window, text='', font=('Arial',8))
    tasks.grid(row=11, column=13, sticky='n')
    #Time increment and label
    time_label = Label(window, text='')
    time_label.grid(row=0, column=0, sticky='w')

    time_increment = Button(window, text='Increment time', command=incrementTime)
    time_increment.grid(row=0, column=1, sticky='w')
    #Memory set button
    memory_entry_lbl = Label(window, text="Memory to set:")
    memory_entry_lbl.grid(row=1, column=0, sticky='w')

    memory_entry = Entry(window)
    memory_entry.grid(row=1,column=1, sticky='w')

    set_memory = Button(window, text='Set memory', command=setMemory)
    set_memory.grid(row=2, column=1, sticky='w')
    #Task add button

    task_size_label = Label(window, text='Task size:')
    task_size_label.grid(row=3, column=0, sticky='w')
    task_size_entry = Entry(window)
    task_size_entry.grid(row=3, column=1, sticky='w')

    task_time_label = Label(window, text='Task lifetime:')
    task_time_label.grid(row=3, column=2)
    task_time_entry = Entry(window)
    task_time_entry.grid(row=3, column=3, sticky='w')

    add_task = Button(window, text='Add a task', command=addTask)
    add_task.grid(row=4, column=1, sticky='w')

    #Task remove button
    task_remove_label = Label(window, text='№ of the task to remove:')
    task_remove_label.grid(row=5, column=0, sticky='w')

    task_remove_entry = Entry(window)
    task_remove_entry.grid(row=5, column=1, sticky='w')

    task_remove_button = Button(window, text='Remove task', command=removeTask)
    task_remove_button.grid(row=6, column=1, sticky='w')

    #Defragmentation button
    defragment_label = Label(window, text='Click to defragment:')
    defragment_label.grid(row=7, column=0, sticky='w')

    defragment_button = Button(window, text='Defragment', command=defragment)
    defragment_button.grid(row=7, column=1, sticky='w')

    update()

    window.mainloop()
