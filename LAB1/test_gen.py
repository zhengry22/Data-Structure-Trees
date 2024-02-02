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
    natural_number = random.randint(0, random_number)
    # 生成一个0到5之间的自然数，你可以根据需要调整范围
    alphabet = 'BA'
    uppercase_letter = random.choice(random.choice(alphabet)) # 从大写字母中随机选择一个
    line = f"{0} B"
    return line

length = 30000

# 生成 "AABB" 交替的字符串
result = "AABB" * (length // 4)

# 如果长度不是4的倍数，添加余下的部分
remainder = length % 4
result += "AABB"[:remainder]
for i in range(2499):
    result += "AB"
result += "A"

#print(result)

random_string = generate_random_string(10000)
print(result)
print(500000)
for i in range(500000):
    print(generate_random_data(i))