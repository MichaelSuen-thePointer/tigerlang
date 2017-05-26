package main

import "log"

func main() {
	produceString()
	err := iterInputs()
	if err != nil {
		log.Println(err)
	}
	build()
}
