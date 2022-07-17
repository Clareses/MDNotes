;SYSSIZE是需要加载的clicks (16 Byte pre click) 的数量
;0x3000拥有196kB的大小，对这个版本的Linux来说已经足够了
SYSSIZE = 0x3000

;	bootsect.s		(C) 1991 Linus Torvalds

;bootsect.s将被BIOS加载到0x7c00,之后它会将自己移动到0x90000,并跳转到对应位置
;(问题在于，为什么要移动和跳转？之前的位置留给了什么？)
;紧接着，它将依次将setup与system加载到自己背后（bootsect大小为512byte，则setup将从0x9200
;开始，而system部分将从0x10000开始）


.globl begtext, begdata, begbss, endtext, enddata, endbss
;这一部分将代码段、数据段、未定义数据段都写在了一起...
.text
begtext:
.data
begdata:
.bss
begbss:
.text

SETUPLEN = 4				;nr of setup-sectors，猜测是setup的扇区数量
BOOTSEG  = 0x07c0			;最开始的时候bootsect的位置
INITSEG  = 0x9000			;初始化段？总之最后将boot移动到了这个位置
SETUPSEG = 0x9020			;setup部分的code段从0x90200开始
SYSSEG   = 0x1000			;系统的代码将从0x10000开始加载
ENDSEG   = SYSSEG + SYSSIZE ;最终停止加载的位置
ROOT_DEV = 0x306 			;指定了磁盘的分区位置（可以通过查读取磁盘分区的参数得知）
							;这是当年Linus第二个硬盘的第一个分区（即启动分区）

entry _start
_start:		
;程序从这里正式开始
	mov	ax,#BOOTSEG 		;这两句，将bootseg（即代码现在所在的段）设置为dataseg的开始
	mov	ds,ax				;正如上文所说，接下去会将bootseg复制到0x90000的位置
	mov	ax,#INITSEG			
	mov	es,ax				;同样地，将当前代码将要移动到的位置（0x90000）保存到extra section
	mov	cx,#256				;在cx中写入256，用于控制rep循环的次数
	sub	si,si				;设置偏移寄存器
	sub	di,di				;同上
	rep						;会重复执行下一条指令，直到cx自减为0
	movw					;一次移动一个字，从ds:si -> es:di
							;上面这些操作将会把BIOS加载进来的整个扇区（512KB）移动到0x90000处
	jmpi	go,INITSEG		;跳转到 INITSEG:go 处

go:	mov	ax,cs				;将cs中的值存入as（cs中的值为0x90000，因为调用了jmp）
	mov	ds,ax				;data section从 0x90000 开始
	mov	es,ax				;extra section从 0x90000 开始			
	mov	ss,ax				;将栈顶设置在0x90000处
	mov	sp,#0xFF00			;将栈底初始化为0x9ff00

;! load the setup-sectors directly after the bootblock.
;! Note that 'es' is already set up.

load_setup:
;这一部分将为调用BIOS磁盘读取中断作准备，查表可以得到以下信息
	mov	dx,#0x0000			;DH选择磁头号，DL选择驱动器号，这里选择了0号驱动的0号磁头
	mov	cx,#0x0002			;CH选择磁盘号，CL选择了起始扇区号，这里是2磁盘的0扇区
	mov	bx,#0x0200			;ES:BX是接收缓冲区,这里ES=0x90000,BX=512KB,刚好跳过了bootsect的大小
	mov	ax,#0x0200+SETUPLEN	;AL扇区数（SETUPLEN = 4,读了四个扇区），AH选择读服务
	int	0x13				;调用BIOS中断读取磁盘
	jnc	ok_load_setup		;如果成功，则跳转到ok_load_setup
	mov	dx,#0x0000
	mov	ax,#0x0000
	int	0x13
	j	load_setup			;否则重新设置参数，并再次读取

ok_load_setup:
;这一部分将获取磁盘参数（特别是磁盘的扇区数和磁道数）
	mov	dl,#0x00		;DL代表查询的驱动器号，这里查询了驱动器0
	mov	ax,#0x0800		;AH选择服务，这里是获取磁盘参数服务
	int	0x13			;中断调用
	mov	ch,#0x00		;CH保存柱面数的高八位，这里把它清空了...
	seg cs				
	mov	sectors,cx		;将cx的值存入了一个字长(32bit)的空间中(sectors是个标量)
	mov	ax,#INITSEG		;将0x90000又放入ax中了
	mov	es,ax			;将es更新为0x90000

	mov	ah,#0x03		;调用了int 10的3号服务（读取光标位置）
	xor	bh,bh			;BH是页号参数，这里利用异或置0了
	int	0x10			;读取光标位置，CH=光标起始行，DH=行，DL=列
	mov	cx,#24
	mov	bx,#0x0007
	mov	bp,#msg1
	mov	ax,#0x1301
	int	0x10			;调用中断，输出msg1：Loading system ...

;再下一步，将读取系统的代码了！
	mov	ax,#SYSSEG	
	mov	es,ax			;将es段的起始地址改为SYSSEG(0x10000)
	call	read_it		;调用了read_it过程（会读入整个操作系统）
	call	kill_motor	

;接下去，会确认根文件系统的设备。如果已经被define了(即ROOT_DEV的值不为0)，则
;跳转到ROOT_DEFINED，否则对设备进行选择（选择取决于上面获取的磁盘信息）

	seg cs
	mov	ax,root_dev
	cmp	ax,#0
	jne	root_defined
	seg cs
	mov	bx,sectors
	mov	ax,#0x0208		
	cmp	bx,#15
	je	root_defined
	mov	ax,#0x021c
	cmp	bx,#18
	je	root_defined
undef_root:
	jmp undef_root
root_defined:
	seg cs
	mov	root_dev,ax

	;如果root也已经define，那么一切准备就绪，可以进入SETUP.s了
	jmpi	0,SETUPSEG

;! This routine loads the system at address 0x10000, making sure
;! no 64kB boundaries are crossed. We try to load it as fast as
;! possible, loading whole tracks whenever we can.
;!
;! in:	es - starting address segment (normally 0x1000)
;!
sread:	.word 1+SETUPLEN	;! sectors read of current track
head:	.word 0			;! current head
track:	.word 0			;! current track

;这一过程大概就是不停操作磁盘（因为一个磁道装不完一个系统...就是要进行蛮多次读取的）
read_it:
	mov ax,es
	test ax,#0x0fff
die:	jne die			;! es must be at 64kB boundary
	xor bx,bx		;! bx is starting address within segment
rp_read:
	mov ax,es
	cmp ax,#ENDSEG		;! have we loaded all yet?
	jb ok1_read
	ret
ok1_read:
	seg cs
	mov ax,sectors
	sub ax,sread
	mov cx,ax
	shl cx,#9
	add cx,bx
	jnc ok2_read
	je ok2_read
	xor ax,ax
	sub ax,bx
	shr ax,#9
ok2_read:
	call read_track
	mov cx,ax
	add ax,sread
	seg cs
	cmp ax,sectors
	jne ok3_read
	mov ax,#1
	sub ax,head
	jne ok4_read
	inc track
ok4_read:
	mov head,ax
	xor ax,ax
ok3_read:
	mov sread,ax
	shl cx,#9
	add bx,cx
	jnc rp_read
	mov ax,es
	add ax,#0x1000
	mov es,ax
	xor bx,bx
	jmp rp_read

read_track:
	push ax
	push bx
	push cx
	push dx
	mov dx,track
	mov cx,sread
	inc cx
	mov ch,dl
	mov dx,head
	mov dh,dl
	mov dl,#0
	and dx,#0x0100
	mov ah,#2
	int 0x13
	jc bad_rt
	pop dx
	pop cx
	pop bx
	pop ax
	ret
bad_rt:	mov ax,#0
	mov dx,#0
	int 0x13
	pop dx
	pop cx
	pop bx
	pop ax
	jmp read_track

;!/*
;! * This procedure turns off the floppy drive motor, so
;! * that we enter the kernel in a known state, and
;! * don't have to worry about it later.
;! */
kill_motor:
	push dx
	mov dx,#0x3f2
	mov al,#0
	outb
	pop dx
	ret

sectors:
	.word 0

msg1:
	.byte 13,10
	.ascii "Loading system ..."
	.byte 13,10,13,10

.org 508
root_dev:
	.word ROOT_DEV
boot_flag:
	.word 0xAA55

.text
endtext:
.data
enddata:
.bss
endbss:
