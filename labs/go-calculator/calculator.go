package main

import (
	"fmt"
	"os"
	"strconv"
)

func add(a int, b int) int {
	return a + b
}

func sub(a int, b int) int {
	return a - b
}

func mult(a int, b int) int {
	return a * b
}

func getFuncOp(operator int) func(int, int) int {
	var opFunc func(int, int) int

	if operator ==1 {
        opFunc = add;
	} else if operator == 2 {
        opFunc = sub;
	} else {
        opFunc = mult;
	}

	return opFunc
}

// adds/subtracts/multiplies all values that are in the *values array.
// nValues is the number of values you're reading from the array
// operator will indicate if it's an addition (1), subtraction (2) or
// multiplication (3)
func calc(operator int, values []int) int {
	result := values[0]
	opFunc := getFuncOp(operator)

	for i := 1; i<len(values); i++ {
		result = opFunc(result, values[i])
	}

	return result
}


func main() {
	N := len(os.Args)

	if N < 3 {
		fmt.Printf("A valid input was not provided.\n")
        fmt.Printf("Usage example `./calculator.c add 1 2 3 4 5`\n")
        os.Exit(1)
	}

	op := os.Args[1]
	if !(op == "add" || op == "sub" || op == "mult") {
		fmt.Printf("A valid operator was not provided.\n")
        fmt.Printf("Available operators: [add | sub | mult]\n")
        os.Exit(1)
	}

	nums := make([]int, N - 2)

	for i := 2; i < N; i++ {
		currNum, err := strconv.ParseInt(os.Args[i], 10, 0)
		if err != nil {
			fmt.Printf("The calculator can only work with integer values!\n")
			fmt.Printf("Usage example `./calculator.c add 1 2 3 4 5`\n")
			os.Exit(1)
		}
		nums[i-2] = int(currNum)
	}

	operatorCode := 0

	if op == "add" {
		operatorCode = 1
	} else if op == "sub" {
		operatorCode = 2
	} else {
		operatorCode = 3
	}

	result := calc(operatorCode, nums)
	fmt.Printf(strconv.Itoa(result) + "\n")
}

