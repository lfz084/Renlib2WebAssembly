const importObject = {
  getBuffer: ()=>{},
  memoryBound:  ()=>{},
  loading:  ()=>{},
  outputSGFCache: ()=>{},
  b: ()=>{},
  c: ()=>{},
  d: ()=>{},
  a: ()=>{},
};

async function _getBuffer(path) {
  return new Promise((resolve, reject) => {
    require("fs").readFile(path, (err, data) => {
      if (err) reject(err);
      else resolve(data);
    })
  })
  
  return fetch(path)
  .then(response => {
    return response.arrayBuffer()
  })
  
}

function loadWASM(url, importObject) {
  _getBuffer(url)
  .then(bytes => {
    return WebAssembly.instantiate(bytes, importObject);
  })
  .then(results => {
    const wasm_exports = results.instance.exports;
    const memory = wasm_exports.memory || wasm_exports.e;
    console.log(wasm_exports);
    console.log(`buffer byteLength: ${memory.buffer.byteLength >>> 20}mb`)
  });
}

loadWASM("./RenLib.wasm", {env: importObject, a: importObject})
