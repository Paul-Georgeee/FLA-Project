import subprocess
import os
import sys
import random

COLORS = {"default": "\033[0m", "red": "\033[31m", "green": "\033[32m"}

def color(name, text):
  if os.isatty(1):
    return COLORS[name] + text + COLORS["default"]
  return text

def run(file, input):
    return subprocess.run(["./bin/turing", file, input],
                        stdout=subprocess.PIPE,
                        stderr=subprocess.PIPE, text=True, encoding="utf-8")

def testcase1():
    print("TEST CASE 1:")
    flag = True
    for _ in range(0, 1000):
        length = random.randint(1 + 1, 10000)
        s = ""
        for _ in range(0, length):
            r = random.randint(0, 100) % 2
            if r == 1:
                s = s + "1"
            else:
                s = s + "0"
        ans = s[-1] + s[0 : len(s) - 1] + '\n'
        tm = run("./programs/case1.tm", s)
        if tm.returncode != 0:
            flag = False
            print(color("red", "RE"), "stderr output:", tm.stderr.strip(), "when input is " ,s)
            break
        else:
            if tm.stdout != ans:
                flag = False
                print(color("red", "WA"), "error when input is", s)
                break
    if flag:
        print("Test1:", color("green", "AC"))

def judge(a):
    if a == 0:
        return True
    for i in range(1, a + 1):
        if(i * i == a):
            return True
    return False

def testcase2():
    print("TEST CASE 2:")
    flag = True
    for i in range(0, 10000):
        r = random.randint(0, 10)
        if r < 7:
            continue
        print("Begin test. Input: ", i)
        tm = run("./programs/case2.tm", "1" * i)
        ans = "true\n" if judge(i) else "false\n"
        if tm.returncode != 0:
            flag = False
            print(color("red", "RE"), "stderr output:", tm.stderr.strip(), "when input is " ,i)
            return
        else:
            if tm.stdout != ans:
                flag = False
                print(color("red", "WA"), "error when input is", i, ",ans is ", ans, ",output is", tm.stdout)
                return
            else:
                print(color("green", "AC"), ":input is", i, " output is ", tm.stdout)
    
    for _ in range(0, 10):
        r = random.randint(10000, 50000)
        print("Begin test. Input: ", r)
        tm = run("./programs/case2.tm", "1" * r)
        ans = "true\n" if judge(r) else "false\n"
        if tm.returncode != 0:
            flag = False
            print(color("red", "RE"), "stderr output:", tm.stderr.strip(), "when input is " ,r)
            return
        else:
            if tm.stdout != ans:
                flag = False
                print(color("red", "WA"), "error when input is", r, ",ans is ", ans, ",output is", tm.stdout)
                return
            else:
                print(color("green", "AC"), ":input is", r, " output is ", tm.stdout)

    for _ in range(0, 10):
        r = random.randint(100, 300)
        print("Begin test. Input: ", r * r)
        tm = run("./programs/case2.tm", "1" * (r * r))
        ans = "true\n"
        if tm.returncode != 0:
            flag = False
            print(color("red", "RE"), "stderr output:", tm.stderr.strip(), "when input is " ,r * r)
            return
        else:
            if tm.stdout != ans:
                flag = False
                print(color("red", "WA"), "error when input is", r * r, ",ans is ", ans, ",output is", tm.stdout)
                return
            else:
                print(color("green", "AC"), ":input is", r * r, " output is ", tm.stdout)
    
    if flag:
        print("Test2:", color("green", "AC"))

def main():
    #testcase1()
    testcase2()
if __name__ == "__main__":
    main()