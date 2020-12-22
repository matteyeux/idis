# iDIS

iBoot Disassembler made for fun with [Capstone](http://www.capstone-engine.org/).

```bash
λ ~/dev/idis(master*) » ./idis -c 20 iBoot-6723.60.60.0.2-iOS-14.3b2 
0x19c030000	adrp 		x0, #0x19c030000
0x19c030004	add 		x0, x0, #0
0x19c030008	ldr 		x1, #0x19c030300
0x19c03000c	bl 		#0x19c07b6d0
0x19c030010	cmp 		x1, x0
0x19c030014	b.eq 		#0x19c030060
0x19c030018	ldr 		x30, #0x19c030048
0x19c03001c	ldr 		x2, #0x19c030308
0x19c030020	sub 		x2, x2, x1
0x19c030024	mov 		x5, x0
0x19c030028	mov 		x6, x2
0x19c03002c	mov 		x7, x1
0x19c030030	ldp 		x3, x4, [x0], #0x10
0x19c030034	stp 		x3, x4, [x1], #0x10
0x19c030038	subs 		x2, x2, #0x10
0x19c03003c	b.ne 		#0x19c030030
0x19c030040	ret 		
0x19c030044	nop 		
0x19c030048	ldr 		q16, #0x19c036050
0x19c03004c	db 		0x01, 0x00, 0x00, 0x00
```
It is recommended to use [next](https://github.com/aquynh/capstone/tree/next) branch of Capstone.

You better use [disarm](http://newosxbook.com/tools/disarm.html)
