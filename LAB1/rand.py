import random
import string
import subprocess
def generate_random_string(length):
    alphabet = 'ABCD'
    result = [random.choice(alphabet) for _ in range(length)]

    # 确保没有三个相同的字母相连
    for i in range(2, length):
        if result[i] == result[i-1] == result[i-2]:
            # 如果出现三个相同字母相连，随机选择一个不同的字母替换
            choices = list(set(alphabet) - {result[i]})
            result[i] = random.choice(choices)

    return ''.join(result)


def generate_random_data(random_number):
    natural_number = random.randint(0, random_number)  # 生成一个0到5之间的自然数，你可以根据需要调整范围
    alphabet = 'ABCD'
    uppercase_letter = random.choice(random.choice(alphabet))   # 从大写字母中随机选择一个
    line = f"{natural_number} {uppercase_letter}"
    return line


counter = 0   
for i in range(0, 10):
    
    # 生成一个长度为 random_number 的字符串
    random_number = random.randint(0, 50)
    random_string = generate_random_string(random_number)
    n = random.randint(1, 10)
    
    # 运行C++程序，并将其输入重定向到Python程序的输出
    cpp_process_test = subprocess.Popen(["01.exe"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
    cpp_process_correct = subprocess.Popen(["Correct_answer.exe"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
    
    # 首先借助正确的程序实现完全随机的输入
    cpp_input = random_string + "\n" + str(n) + "\n"
    cpp_process_correct.stdin.write(cpp_input)
    cpp_process_correct.stdin.close()
    my_len = random.randint(0, random_number)
    
    print(random_number, random_string)
    print(n)
    for i in range(n):
        # 随机生成每次需要插入的数据
        cpp_process_correct = subprocess.Popen(["Correct_answer.exe"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
        line = generate_random_data(my_len)
        print("line is: ", line)
        
        cpp_process_correct.stdin.write(str(line) + "\n")
        cpp_process_correct.stdin.flush()
        cpp_input = cpp_input + line + "\n"
        output = cpp_process_correct.stdout.read()
        print("in cycle! ")
        print(output)
        cpp_process_correct.stdin.close()
        
        # 再获取这次输入所得到的输出
        my_len = output
        print("new my_len is: ", my_len)

    print("our input is: ", cpp_input)
    cpp_process_correct = subprocess.Popen(["Correct_answer.exe"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
    cpp_process_correct.stdin.write('a')
    correct_output = cpp_process_correct.stdout.read()
    print("our correct output is: ", correct_output)
    
    cpp_process_correct.stdin.close()
    
    
    # 将数据写入C++程序的标准输入
    cpp_process_test.stdin.write(cpp_input)
    cpp_process_test.stdin.close()

    # 读取C++程序的输出
    cpp_output_test = cpp_process_test.stdout.read()

    if (cpp_output_test != correct_output) :
        #output_file = open("wrong_cases.txt", "w")
        counter += 1
        print("our input is", cpp_input)
        print("Wrong Answer: ")
        print("C++ Program Output:")
        print(cpp_output_test)
        print("Correct Output:")
        print(correct_output)
        print("\n")
        
    # 等待C++程序完成并获取返回代码
    cpp_return_code = cpp_process_test.wait()

print(f"there are {counter} wrong answers")