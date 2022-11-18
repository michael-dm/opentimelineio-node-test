const Timeline = require('..')
const assert = require('assert')
const fs = require('fs-extra')
const path = require('path')

assert(Timeline, 'The expected module is undefined')

async function testBasic() {
  const otioFile = await fs.readFile(path.join(__dirname, 'example.otio'), { encoding: 'utf-8' })
  const instance = new Timeline(otioFile)
  //assert(instance.greet, 'The expected method is not defined')
  //assert.strictEqual(instance.greet('kermit'), 'mr-yeoman', 'Unexpected value returned')
}

function testInvalidParams() {
  const instance = new Timeline()
}

assert.doesNotThrow(testBasic, undefined, 'testBasic threw an expection')
assert.throws(testInvalidParams, undefined, "testInvalidParams didn't throw")

console.log('Tests passed- everything looks OK!')
