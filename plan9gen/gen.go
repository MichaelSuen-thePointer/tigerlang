package main

import (
	"io/ioutil"
	"strings"
	"log"
	"os"
	"bufio"
	"encoding/json"
	"io"
	"fmt"
	"strconv"
	"os/exec"
)

var stringTable = make(map[string]string)

func produceString() error {
	f, err := os.OpenFile("./in/strings.json", os.O_RDONLY, 0666)
	if err != nil {
		return err
	}
	defer f.Close()
	reader := bufio.NewReader(f)

	d := json.NewDecoder(reader)
	if err := d.Decode(&stringTable); err != nil {
		return err
	}

	stringF, err := os.OpenFile("./out/strings.s", os.O_CREATE | os.O_RDWR | os.O_TRUNC, 0666)
	if err != nil {
		return err
	}
	writer := bufio.NewWriter(stringF)
	defer writer.Flush()
	defer stringF.Close()

	for k, v := range stringTable {
		for i := 0; i < len(v); i++ {
			quoted := strconv.Quote(string([]byte{v[i]}))
			fmt.Fprintf(stringF, "DATA %s+%d(SB)/1, $'%s'\n", k, i, quoted[1:len(quoted)-1])
		}
		fmt.Fprintf(stringF, "GLOBL %s(SB), (8+16), $%d\n\n", k, len(v))
	}

	return nil
}

func iterInputs() error {
	argsF, err := os.OpenFile("./out/args.go", os.O_CREATE | os.O_TRUNC | os.O_RDWR, 0666)
	if err != nil { return err }
	argsF.Write([]byte("package main\n\n"))
	argsF.Close()

	files, err := ioutil.ReadDir("./in")
	if err != nil { return err }
	for _, fi := range files {
		fname := fi.Name()
		fname_s := strings.Split(fname, ".")
		if len(fname_s) == 2 && fname_s[1] == "json" {
			if fname[0] == 'I' && fname[1] == 'R' {
				if err := compile(fname_s[0][2:]); err != nil {
					return err
				}
			}
		}
	}
	return nil
}

type function struct {
	name string
	out io.Writer
	stackSize int
	paramSize int
	inParam int // static link considered
	outParam int // static link considered
	localVar int
	irt map[string]interface{}
	tempId int
}

func (fun *function) getTempRegister() string {
	fun.tempId++
	if fun.tempId % 2 == 0 {
		return "AX"
	}
	return "BX"
}

func compile(inname string) error {
	fname := "./in/IR" + inname + ".json"
	log.Println("Compiling: " + fname)
	f, err := os.Open(fname)
	if err != nil {
		return err
	}

	reader := bufio.NewReader(f)
	defer f.Close()

	irt := make(map[string]interface{})
	d := json.NewDecoder(reader)
	err = d.Decode(&irt)
	if err != nil {
		return err
	}

	outf, err := os.OpenFile("./out/" + inname + ".s", os.O_CREATE | os.O_RDWR | os.O_TRUNC, 0666)
	if err != nil {
		return err
	}
	out := bufio.NewWriter(outf)
	fun := &function {
		name:inname, out:out, inParam:1, irt:irt,
	}
	defer outf.Close()
	defer out.Flush()

	if err := fun.Evaluate(); err != nil {
		return err
	}
	return fun.Compile()
}

func (fun *function) Evaluate() error {
	err := fun.evaluateIRT(fun.irt)
	fun.paramSize = 8 * fun.inParam
	fun.stackSize = (fun.localVar + fun.outParam) * 8
	if err != nil {
		return err
	}
	log.Println("inParam:", fun.inParam, "outParam:", fun.outParam, "localVars:", fun.localVar)
	log.Println("paramSize:", fun.paramSize, "stackSize:", fun.stackSize)

	argsF, err := os.OpenFile("./out/args.go", os.O_APPEND | os.O_RDWR, 0666)
	if err != nil { return err }
	defer argsF.Close()
	writer := bufio.NewWriter(argsF)
	defer writer.Flush()
	fmt.Fprintf(writer, "func %s(", fun.name)
	for i := 0; i < fun.inParam; i++ {
		if i != 0 {
			fmt.Fprint(writer, " ,")
		}
		fmt.Fprint(writer, "uint64")
	}
	fmt.Fprintln(writer, ") uint64")

	return nil
}

func (fun *function) evaluateIRT(irt map[string]interface{}) (err error) {
	defer func() {
		err, _ = recover().(error)
	} ()

	typev := irt["Type"]
	types := typev.(string)
	switch types {
	case "Seq":
		fun.evaluateIRT(irt["Left"].(map[string]interface{}))
		fun.evaluateIRT(irt["Right"].(map[string]interface{}))
	case "CJump":
		fun.evaluateIRT(irt["Left"].(map[string]interface{}))
		fun.evaluateIRT(irt["Right"].(map[string]interface{}))
	case "Mem":
		fun.evaluateIRT(irt["Offset"].(map[string]interface{}))
	case "Binop":
		//log.Println(irt)
		if irt["Left"] != nil {
			left := irt["Left"].(map[string]interface{})
			if left["Type"].(string) == "TempVar" {
				if irt["Right"] != nil {
					right := irt["Right"].(map[string]interface{})
					if right["Type"].(string) == "Constant" {
						value := int(right["Value"].(float64))
						if left["Id"].(string) == "sp" {
							localVar := value / 8
							if localVar > fun.localVar {
								fun.localVar = localVar
							}
						} else if left["Id"].(string) == "fp" {
							inParam := value / 8
							if inParam > fun.inParam {
								fun.inParam = inParam
							}
						}
					}
				}
			}
		} else {
			fun.evaluateIRT(irt["Left"].(map[string]interface{}))
			fun.evaluateIRT(irt["Right"].(map[string]interface{}))
		}
	case "Constant":
	case "Label":
	case "FunctionCall":
		pnum := 0
		switch irt["Name"] {
		case "print":
			pnum = 2 // param string need 2 words
		case "builtin_initarray":
			pnum = 3 // param * 2 + return 1
		default:
			if irt["Params"] != nil {
				params := irt["Params"].([]interface{})
				pnum = len(params) + 2
				// Why 2? static chain & return value
			}
		}
			if pnum > fun.outParam {
				fun.outParam = pnum
			}
	case "IgnoreValue":
		fun.evaluateIRT(irt["Exp"].(map[string]interface{}))
	case "Jump":
	case "Eseq":
		fun.evaluateIRT(irt["Left"].(map[string]interface{}))
		fun.evaluateIRT(irt["Right"].(map[string]interface{}))
	case "Move":
		fun.evaluateIRT(irt["Left"].(map[string]interface{}))
		fun.evaluateIRT(irt["Right"].(map[string]interface{}))
	case "TempVar":
	default:
		log.Fatal(types + " not implement!")
	}
	return nil
}

func (fun *function) Compile() error {
	fmt.Fprintf(fun.out, "TEXT ·%s(SB), $%d-%d\n", fun.name, fun.stackSize, fun.paramSize)
	_, err := fun.compileIRT(fun.irt)
	fmt.Fprintln(fun.out, "RET")
	return err
}

func (fun *function) compileIRT(irt map[string]interface{}) (val string, err error) {
	defer func() {
		err, _ = recover().(error)
	} ()
	typev := irt["Type"]
	types := typev.(string)
	var lval, rval string
	switch types {
	case "Eseq":
		fun.compileIRT(irt["Left"].(map[string]interface{}))
		return fun.compileIRT(irt["Right"].(map[string]interface{}))
	case "Seq":
		fun.compileIRT(irt["Left"].(map[string]interface{}))
		fun.compileIRT(irt["Right"].(map[string]interface{}))
	case "CJump":
		lval, err = fun.compileIRT(irt["Left"].(map[string]interface{}))
		rval, err = fun.compileIRT(irt["Right"].(map[string]interface{}))
		if lval[0] == '(' && lval[len(lval)-1] == ')' {
			lval = lval[1:len(lval)-1]
			fmt.Fprintf(fun.out, "MOVQ (%s), %s\n", lval, lval)
		}
		if rval[0] == '(' && rval[len(rval)-1] == ')' {
			rval = rval[1:len(rval)-1]
			fmt.Fprintf(fun.out, "MOVQ (%s), %s\n", rval, rval)
		}
		fmt.Fprintf(fun.out, "CMPQ %s, %s\n", lval, rval)
		var op string
		switch irt["op"].(string) {
		case "eq":
			op = "JEQ"
		case "ne":
			op = "JNE"
		case "lt":
			op = "JCS"
		case "gt":
			op = "JHI"
		case "le":
			op = "JLS"
		case "ge":
			op = "JCC"
		}
		fmt.Fprintf(fun.out, "%s %s\n", op, irt["True"])
		fmt.Fprintf(fun.out, "JMP %s\n", irt["False"])
	case "Binop":
		var name, op string
		left := irt["Left"].(map[string]interface{})
		right := irt["Right"].(map[string]interface{})

		switch irt["op"].(string) {
		case "N5tiger2ir4PlusE":
			op = "ADDQ"
		case "N5tiger2ir5MinusE":
			op = "SUBQ"
		case "N5tiger2ir8MultiplyE":
			op = "IMULQ"
		}

		name, err = fun.compileIRT(left)
		if err != nil { return }
		register := fun.getTempRegister()
		fmt.Fprintf(fun.out, "MOVQ %s, %s\n", name, register)
		if right["Type"].(string) == "Constant" {
			value := int(right["Value"].(float64))
			fmt.Fprintf(fun.out, "%s $%d, %s\n", op, value, register)
			return register, nil
		} else {
			fmt.Fprintf(fun.out, "MOVQ %s, CX\n", name)
			name, err = fun.compileIRT(right)
			if err != nil { return }
			fmt.Fprintf(fun.out, "%s %s, CX\n", op, name)
			register = fun.getTempRegister()
			fmt.Fprintf(fun.out, "MOVQ CX, %s\n", register)
			return register, nil
		}
	case "Mem":
		offset := irt["Offset"].(map[string]interface{})
		left := offset["Left"].(map[string]interface{})
		right := offset["Right"].(map[string]interface{})

		switch offset["Type"].(string) {
		case "Binop":
			if left["Type"].(string) == "TempVar" {
				id := left["Id"].(string)
				if right["Type"].(string) == "Constant" {
					value := int(right["Value"].(float64))

					tempid := value
					if tempid < 0 {
						tempid = -tempid
					}
					tempid /= 8

					switch offset["op"].(string) {
					case "N5tiger2ir4PlusE":
						if id == "fp" {
							if value < 0 {
								value -= 8
								register := fun.getTempRegister()
								fmt.Fprintf(fun.out, "LEAQ param%d%+d(SP), %s\n", tempid, value, register)
								return "(" + register + ")", nil
							} else {
								value -= 16
								register := fun.getTempRegister()
								fmt.Fprintf(fun.out, "LEAQ param%d%+d(FP), %s\n", tempid, value, register)
								return "(" + register + ")", nil
							}
						} else {
							// sp do nothing
						}
					case "N5tiger2ir5MinusE":
						log.Fatalln("%s - num was found", id)
					}
				} else {
					log.Fatalln("tempVar + not constant")
				}
			} else if left["Type"].(string) == "Mem" {
				var name string
				name, err = fun.compileIRT(left)
				if err != nil { return }
				fmt.Fprintf(fun.out, "MOVQ %s, CX\n", name)
				if right["Type"].(string) == "Constant" {
					value := int(right["Value"].(float64))
					switch offset["op"].(string) {
					case "N5tiger2ir4PlusE":
						fmt.Fprintf(fun.out, "ADDQ $%d, CX\n", value)
						register := fun.getTempRegister()
						fmt.Fprintf(fun.out, "MOVQ CX, %s\n", register)
						return "(" + register + ")", nil
					case "N5tiger2ir5MinusE":
						log.Fatalln("mem - num was found")
					}
				} else {
					name, err = fun.compileIRT(right)
					if err != nil { return }
					fmt.Fprintf(fun.out, "ADDQ %s, CX\n", name)
					register := fun.getTempRegister()
					fmt.Fprintf(fun.out, "MOVQ CX, %s\n", register)
					return "(" + register + ")", nil
				}
			}
		case "TempVar":
			if left["Id"] == "fp" {
				return "param0-8(SP)", nil
			} else {
				log.Fatalln("mem: tempvar not fp")
			}
		}
	case "Label":
		label := irt["Name"].(string)
		fmt.Fprintf(fun.out, "%s:\n", label)
	case "IgnoreValue":
		fun.compileIRT(irt["Exp"].(map[string]interface{}))
		return
	case "Constant":
		value := int(irt["Value"].(float64))
		return fmt.Sprintf("$%d", value), nil
	case "TempVar":
		id := irt["Id"].(string)
		if id == "fp" {
			register := fun.getTempRegister()
			fmt.Fprintf(fun.out, "MOVQ param0-8(SP), %s\n", register)
			return register, nil
		}
		//return strings.ToUpper(id), nil
		return "DX", nil //TODO: dispatch register
	case "Move":
		var lname, rname string
		lname, err = fun.compileIRT(irt["Left"].(map[string]interface{}))
		if err != nil { return }
		fmt.Fprintf(fun.out,"MOVQ %s, CX\n", lname)
		rname, err = fun.compileIRT(irt["Right"].(map[string]interface{}))
		if err != nil { return }
		if rname[0] == '(' && rname[len(rname)-1] == ')' {
			rname = rname[1:len(rname)-1]
			fmt.Fprintf(fun.out,"MOVQ (%s), %s\n", rname, rname)
		}
		fmt.Fprintf(fun.out,"MOVQ %s, (CX)\n", rname)
		// return
	case "FunctionCall":
		name := irt["Name"].(map[string]interface{})
		if name["Type"] == "Name" {
			switch name["Name"].(string) {
			//butin first
			case "print":
				params := irt["Params"].([]interface{})
				if len(params) != 1 { return }
				param := params[0].(map[string]interface{})
				var pname string
				pname, err = fun.compileIRT(param)
				if err != nil { return }
				fmt.Fprintf(fun.out, "MOVQ %s, str-%d(SP)\n", pname, fun.stackSize)
				fmt.Fprintf(fun.out, "MOVQ CX, strlen-%d(SP)\n", fun.stackSize - 8)
				fmt.Fprintln(fun.out, "CALL ·prints(SB)")
				//no return
			case "builtin_initarray":
				params := irt["Params"].([]interface{})
				if len(params) != 2 { return }
				var p0name, p1name string
				param0 := params[0].(map[string]interface{})
				p0name, err = fun.compileIRT(param0)
				if err != nil { return }
				param1 := params[1].(map[string]interface{})
				p1name, err = fun.compileIRT(param1)
				if err != nil { return }
				// temp register error
				if p0name[0] == '(' && p0name[len(p0name)-1] == ')' {
					p0name = p0name[1:len(p0name)-1]
					fmt.Fprintf(fun.out, "MOVQ (%s), %s\n", p0name, p0name)
				}
				if p1name[0] == '(' && p1name[len(p1name)-1] == ')' {
					p1name = p0name[1:len(p1name)-1]
					fmt.Fprintf(fun.out, "MOVQ (%s), %s\n", p1name, p1name)
				}
				fmt.Fprintf(fun.out, "MOVQ %s, len-%d(SP)\n", p0name, fun.stackSize)
				fmt.Fprintf(fun.out, "MOVQ %s, val-%d(SP)\n", p1name, fun.stackSize - 8)
				fmt.Fprintln(fun.out, "CALL ·builtin_initarray(SB)")
				register := fun.getTempRegister()
				fmt.Fprintf(fun.out, "MOVQ ret-%d(SP), %s\n", fun.stackSize - 16, register)
				return register, nil
			default:
				fmt.Fprintln(fun.out, "LEAQ irfp+8(SP), CX")
				fmt.Fprintf(fun.out, "MOVQ CX, staticlink-%d(SP)\n", fun.stackSize)
				params := irt["Params"].([]interface{})
				for i := range params {
					var name string
					name, err = fun.compileIRT(params[i].(map[string]interface{}))
					if err != nil { return }
					if name[0] == '(' && name[len(name)-1] == ')' {
						name = name[1:len(name)-1]
						fmt.Fprintf(fun.out, "MOVQ (%s), %s\n", name, name)
					}
					offset := - fun.stackSize + 8 * (i + 1)
					fmt.Fprintf(fun.out, "MOVQ %s, param%d%+d(SP)\n", name, i+1, offset)
				}
				fmt.Fprintf(fun.out, "CALL ·%s(SB)\n", name["Name"].(string))
				register := fun.getTempRegister()
				fmt.Fprintf(fun.out, "MOVQ ret-%d(SP), %s\n", fun.stackSize - (len(params)+1) * 8, register)
				return register, nil
			}
		}
	case "JUMP":
		target := irt["Target"].(map[string]interface{})
		if target["Type"].(string) == "Label" {
			lname := target["Name"].(string)
			fmt.Fprintf(fun.out, "JMP %s\n", lname)
		}
	case "Name":
		if v, ok := stringTable[irt["Name"].(string)]; ok {
			register := fun.getTempRegister()
			fmt.Fprintf(fun.out, "LEAQ %s(SB), %s\n", irt["Name"].(string), register)
			fmt.Fprintf(fun.out, "MOVQ $%d, CX\n", len(v))
			return register, nil
		}
	}
	return
}

func build() {
	log.Println("Generating executable file...")
	cmd := exec.Command("./bin/go.exe", "build", "-o", "./out/out.exe", "./out")
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	cmd.Run()
}