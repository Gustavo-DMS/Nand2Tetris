// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed.
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.
    @8192
    D=A
    @R0
    M=D

(KBDLOOP)
    @KBD
    D=M
    @CLEARLOOP
    D;JEQ
    @FILLLOOP
    D;JNE

(FILLLOOP)
    @R0
    D=M
    @R1
    D=M-D
    @KBDLOOP
    D;JEQ

    @SCREEN
    D=A
    @R1
    D=D+M
    M=M+1
    A=D
    M=-1


    @KBDLOOP
    0;JMP

(CLEARLOOP)
    @R1
    D=M
    @KBDLOOP
    D;JLT

    @SCREEN
    D=A+D
    @R1
    M=M-1
    A=D
    M=0


    @KBDLOOP
    0;JMP
