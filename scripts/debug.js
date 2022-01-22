/**
 * [npm debug script]
 * Purpose is to automatically detect Node process architecture and run the
 * corresponding script to build the library for debugging.
 * Defaults to `node-gyp rebuild` if unable to detect the architecture.
 */

/* eslint-disable no-console */
const { spawn } = require('child_process');

function run(script) {
  console.log(`Node architecture is ${process.arch}: running "${script}"`);

  const program = script.split(' ')[0];
  const args = script.split(' ').slice(1);

  // inherit stdio to print colour (helpful for warnings/errors readability)
  const child = spawn(program, args, { stdio: 'inherit' });

  child.on('close', code => console.log(`Script "${script}" exited with ${code}`));
}

const buildScripts = {
  x64: 'run debug64',
  ia32: 'run debug32',
};

if (Object.prototype.hasOwnProperty.call(buildScripts, process.arch)) {
  // on Windows, npm is actually `npm.cmd`
  const npm = /^win/.test(process.platform) ? 'npm.cmd' : 'npm';
  run(`${npm} ${buildScripts[process.arch]}`);
} else {
  console.log('Unfamiliar architecture detected, this library is probably not compatible with your OS.');
  run('node-gyp --debug configure rebuild');
}
