package main

import (
	"fmt"
	"net"
	"strings"
	"os"
	"time"
)

func main() {
	if len(os.Args) == 1 {
		fmt.Printf("Error")
		return
	}
	for _, input := range os.Args[1:] {
		splittedInput := strings.Split(input, "=")

		// fmt.Printf("%s", splittedInput)

		if len(splittedInput) != 2 {
			fmt.Printf("Bad input\n")
			return
		}

		tz := splittedInput[0]
		port := splittedInput[1]

		// fmt.Printf("%s", tz)

		conn, err := net.Dial("tcp", port)

		if err != nil {
			fmt.Printf("%s", err)
			return
		}

		
		go Clock{
			tz:	tz,
			connection: conn,
		}.handleConnection()
	}

	for {
		time.Sleep(1 * time.Second)
	}
}


type Clock struct {
	tz      string
	connection net.Conn
}

func (clock Clock) handleConnection() {
	for {
		data := make([]byte, 11)
		_, err := clock.connection.Read(data)
		if err != nil {
			clock.connection.Close()
			fmt.Printf("%s", err)
			return

		} else {
			fmt.Printf("%s: %s", clock.tz, data)
		}
	}
}