import { readFileSync, readdirSync, statSync } from 'fs'

let gOk = true

const files = readdirSync('./src', { recursive: true })
	.filter(p => p.endsWith('.c'))
	.map(p => `./src/${p}`)
	.map(p => ({ path: p, stats: statSync(p) }))
	.sort((a, b) => a.stats.mtime - b.stats.mtime)

for (const { path, stats } of files) {
	const code = readFileSync(path).toString()
	const functions = code.match(/.*\w+\(.*\)\s*{(?:.|\n)*?\n}/g) || []

	const file = path.split('/').pop().split('.')[0]

	for (const fn of functions) {
		const name = fn.substring(0, fn.indexOf('(')).split(' ').pop()
		const clean = fn.replace(/(?:\/\*[^*]*\*\/)|(?:\/\/.*)|\s/g, '')

		const ok = clean.length <= 555
		gOk = gOk && ok

		if (!ok || process.argv[2] === 'print') console.error(`${file}::${name} - ${clean.length}ch`)
	}
}

if (!gOk) process.exit(1)
