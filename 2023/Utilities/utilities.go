package utilities

func PanicOnError(err error) {
	if err != nil {
		panic(err)
	}
}

func PanicIf(exp bool, msg string) {
	if exp {
		panic(msg)
	}
}
