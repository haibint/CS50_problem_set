
def main():
    while True:
        print("What's your credit card number")
        number = input()
        if len(number)>0:
            break

    single_digits_sum = 0;
    double_digits_sum = 0;

    for i in range(len(number)):
        if i%2 == 0:   # for those which are in single digtis, as i starts counting from 0.
            single_digits_sum += int(number[i])
        if i%2 !=0 :     # for those which are in double digits, as i starts counting from0.
            mult_2 = int(number[i])*2
            if mult_2 >= 10:
                double_digits_sum += (1+(mult_2-10))

            if mult_2 < 10:
                double_digits_sum += mult_2
    total_sum = single_digits_sum + double_digits_sum
    # print(total_sum)

    if total_sum % 10 == 0 and int(number[0]) == 3 and len(number) == 15:
        print("AMEX")
    elif total_sum % 10 == 0 and int(number[0]) == 5 and len(number) == 16:
        print("MASTERCARD")
    elif total_sum % 10 == 0 and int(number[0]) == 4:
        print("VISA")
    else:
        print("INVALID")

if __name__ == "__main__":
    main()