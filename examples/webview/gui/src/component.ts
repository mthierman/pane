import { AddressBar } from "./component/address_bar";
import { Button } from "./component/button";

export class App {
    body = document.body as HTMLBodyElement;

    constructor() {
        Button.define();
        AddressBar.define();
    }

    static new() {
        return new App();
    }

    addElement<T extends Node>(element: T) {
        return this.body.appendChild(element);
    }
}

export { AddressBar, Button };
