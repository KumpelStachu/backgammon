a = ['BLACK', 'RED', 'GREEN', 'YELLOW', 'BLUE', 'MAGENTA', 'CYAN', 'WHITE']
o = ''
for (let i = 0; i < a.length; i++) {
	for (let j = 0; j < a.length; j++) {
		o += `X(0x${i}${j},${a[i]},${a[j]})\\\n`
	}
}
console.log(o)
