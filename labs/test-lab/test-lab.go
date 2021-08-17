package main

import (
	"fmt"
	"os"
	"strings"
)

func main() {
  user_args := os.Args[1:]
  user := strings.Join(user_args ," ")

  
  if len(user) == 0 {
    	fmt.Println("Error: You didn't provide a valid user name.")
    	fmt.Println("Usage: 'go run test-lab.go <user name>'")
    	return
  }
  
  
	fmt.Println("Hello " + user + ", Welcome to the Jungle")
}
