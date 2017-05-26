package main

import (
	"fmt"
	"unsafe"
)

func prints(str string) {
	fmt.Print(str)
}

func builtin_initarray(n, v int) uintptr {
	a := make([]int, n, v)
	return uintptr(unsafe.Pointer(&a[0]))
}

func main() {
	Out(0)
}