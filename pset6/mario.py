
def main():
    while True:
        print("How many layers do you want to have for mario pyramids?  ")
        n = int(input())
        if n>0 and n<23:
            break

    for i in range(n+1):
        print(""*(n-i), end='')        # print out blanks before any #
        print("#"*i, end='')           # print i number of #
        print(" "*2, end='')           # print two spaces in between
        print("#"*i)                   # print i number of # again


if __name__ == "__main__":
    main()