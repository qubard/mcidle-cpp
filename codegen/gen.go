package main

import (
	"os"
    "strings"
	"text/template"
    "bufio"
    "fmt"
)

type Field struct {
    Type string
    Name string
}

type PacketInfo struct {
    Name        string
    Fields      []Field
}

func main() {
    os.Mkdir("out", 0777)
    variables := []Field{}

    reader := bufio.NewReader(os.Stdin)
    name, _ := reader.ReadString('\n')
    name = strings.TrimSpace(name)
    for {
        text, _ := reader.ReadString('\n')
        text = strings.TrimSpace(text)
        tokens := strings.Split(text, " ")

        if len(text) == 0 {
            break
        }

        if len(tokens) != 2 {
            fmt.Println(text, tokens, len(tokens))
            break
        }

        variables = append(variables, Field { tokens[0], tokens[1] })
    }
    td := PacketInfo{Name: name, Fields: variables}

    t, err := template.New("clientbound-header.tmpl").ParseFiles("clientbound-header.tmpl")
    if err != nil {
        panic(err)
    }
    f, err := os.Create(fmt.Sprintf("out/%s.hpp", name))
    if err != nil {
        panic(err)
    }
    defer f.Close()
    err = t.Execute(f, td)
    if err != nil {
        panic(err)
    }

    t, err = template.New("clientbound-body.tmpl").ParseFiles("clientbound-body.tmpl")
    if err != nil {
        panic(err)
    }
    f2, err := os.Create(fmt.Sprintf("out/%s.cpp", name))
    defer f2.Close()
    if err != nil {
        panic(err)
    }
    err = t.Execute(f2, td)
    if err != nil {
        panic(err)
    }
    fmt.Println(fmt.Sprintf("Done writing %s", name))
}
