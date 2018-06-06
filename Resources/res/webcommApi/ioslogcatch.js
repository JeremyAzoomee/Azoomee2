// Debug
// This file is to be included in games index.html files, to redefine console.log

console = new Object();
console.log = function(log) {
  parent.postMessage({method: "sendLog", responseID: "sendLog", logString: log},"*");
};

console.debug = console.log;
console.info = console.log;
console.warn = console.log;
console.error = console.log;
