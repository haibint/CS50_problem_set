import sys

def main():
    key = int(sys.argv[1])
    if key>0:
        shift = key%27
        print(shift)
    else:
        print("invalid argument")
        exit(1)
    print("Your plaintext input")
    string = input()

    for c in string:
        if c.isalpha() and c.isupper():
            ascii = ord(c)
            tem = ascii - 65 + shift
            cipher_char = tem % 26
            print(chr(cipher_char + 65), end="")
        elif c.isalpha() and c.islower():
            ascii = ord(c)
            tem = ascii - 97 + shift
            cipher_char = tem % 26
            print(chr(cipher_char + 97), end="")
        else:
            print(c, end="")
    print("")

if __name__ == "__main__":
    main()