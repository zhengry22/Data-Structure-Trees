import subprocess
import random
import time
import random

num_random_numbers = 10  # 指定生成的随机数的数量
min_value = 1  # 最小值
max_value = 5  # 最大值
current_time = int(time.time())
random.seed(current_time)
random_integer = random.randint(min_value, max_value)
random_numbers = [random.randint(min_value, max_value) for _ in range(num_random_numbers)]
random_numbers.insert(0, random_integer)
random_numbers.insert(0, num_random_numbers)
print(f"random numbers are: {random_numbers}")
# 运行C++程序，并将其输入重定向到Python程序的输出

# 将random_numbers列表转换为适当格式的字符串
cpp_input_data = ' '.join(map(str, random_numbers))

cpp_process = subprocess.Popen(["./search_test.exe"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
    
# 将数据写入C++程序的标准输入
cpp_process.stdin.write(cpp_input_data)
cpp_process.stdin.close()

# 读取C++程序的输出
cpp_output = cpp_process.stdout.read()

# 打印C++程序的输出

print("output of the result is:", cpp_output)
        
# 等待C++程序完成并获取返回代码
cpp_return_code = cpp_process.wait()
    