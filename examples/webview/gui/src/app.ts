import { Component } from "./component";

export class App {
    html = document.documentElement as HTMLHtmlElement;
    body = document.body as HTMLBodyElement;

    constructor() {
        Component.register();
    }

    static new() {
        return new App();
    }

    addElement<T extends Node>(element: T) {
        return this.body.appendChild(element);
    }
}
