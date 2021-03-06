# Carrier density

This program was written during the Physical Fundamentals of Microelectronics course in the 4th year of the Physics Department at Novosibirsk State University.

Here you can find the [task]

# Getting Started

  - Install [Golang]
  - Install [Qt bindings for Golang] (tip: you need Qt 5.11.1 installed)

# Useful links
  - [GitHub Getting Started]
  - [GitHub Forking Projects]
  
  - [An Introduction to Programming in Go] (или на [русском](http://golang-book.ru/))
  - [Golang code-style tips]
  - An introduction to [Golang code organization]
  
  - [Golang Qt examples]
  - [Qt documentation]


# Deployment

## Linux to Linux

```sh
qtdeploy build desktop
```

You should find your deployed application inside the deploy subfolder.

Please use the \*.sh file to start your application.

## Linux to Windows 32 bit Shared

- optional: Install Wine if you want to test your Windows application on Linux

- Pull the target image

```sh
docker pull therecipe/qt:windows_32_shared
```

- Deploy your application

```sh
qtdeploy -docker build windows_32_shared
```

You should find your deployed application inside the deploy subfolder.

## Windows to Windows 32 bit Shared

```sh
qtdeploy build desktop
```

You should find your deployed application inside the deploy subfolder.


### Todos

  - Add Travis CI

[//]: # (These are reference links used in the body of this note and get stripped out when the markdown processor does its job. There is no need to format nicely because it shouldn't be seen. Thanks SO - http://stackoverflow.com/questions/4823468/store-comments-in-markdown-syntax)


   [task]: <https://github.com/moonlightnvkz/CarrierDensity/blob/master/task.pdf>
   [Golang]: <https://golang.org/doc/install>
   [Qt bindings for Golang]: <https://github.com/therecipe/qt>
   [An Introduction to Programming in Go]: <https://www.golang-book.com/books/intro>
   [Golang code-style tips]: <https://golang.org/doc/effective_go.html>
   [Golang code organization]: <https://golang.org/doc/code.html>
   [GitHub Getting Started]: <https://guides.github.com/activities/hello-world/>
   [GitHub Forking Projects]: <https://guides.github.com/activities/forking/>
   [Golang Qt examples]: <https://github.com/therecipe/examples>
   [Qt documentation]: <https://doc.qt.io/qt-5.11/>
