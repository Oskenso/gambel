"use strict";
var M = require('./libgambel.js');

class Memory {
	constructor() {
	}
}

//Registers Structure
class Registers {
	constructor(ptr) {
		this.PTR = ptr;
		this.A = M.getValue(ptr, 'i8');
		this.F = M.getValue(ptr, 'i8');

		this.Memory = M.getValue(ptr+12, '*')//64*1024
	}
}

class CPU {
	constructor(ptr) {
		this._ptr = ptr;
		this.Registers = new Registers(this._ptr);
	}
}

module.exports.GB = class GB {

	constructor() {
		this._ptr = M._GB_Create();
		//this.CPU = M.getValue(this.CPU, '*');
		this.CPU = new CPU(M.getValue(this._ptr, '*'));
	}

	//get cpu() {}

	GB_Destroy(ptr) {
		return M._GB_Destroy(this.GB);
	}
}

