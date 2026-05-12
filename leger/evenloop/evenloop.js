queueMicrotask(callback) {
    if (typeof callback !== "function") {
        throw new TypeError("callback is not a function");
    }
    this.microTaskQueue.push(callback);
}

setTimeout(callback, delay, ...params) {
    if (typeof callback !== "function") {
        throw new TypeError("callback is not a function");
    }
    if (delay === undefined) {
        delay = 0;
    }
    const executeAt = Date.now() + delay;
    sortedInsert(this.macroTaskQueue, { executeAt, callback, params }, "executeAt");
}

executeMicroTasks() {
    while (this.microTaskQueue.length > 0) {
        const task = this.microTaskQueue.shift();
        task();
    }
}

executeMacroTask() {
    if (this.macroTaskQueue.length > 0 && this.macroTaskQueue[0].executeAt <= Date.now()) {
        const { callback, params } = this.macroTaskQueue.shift();
        callback(...params);
    }
}
