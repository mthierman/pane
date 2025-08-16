import { Utility } from "../utility";
import { Component } from "./component";

declare global {
    interface HTMLElementTagNameMap {
        [Button.tag]: Button;
    }
}

export class Button extends Component {
    static tag = "pane-button";

    #button = document.createElement("button") as HTMLButtonElement;

    static new() {
        Button.define(Button.tag, this);
        let element = document.createElement(Button.tag) as Button;
        Utility.addElement(element);
        return element;
    }

    connectedCallback() {
        this.appendChild(this.#button);

        this.addEventListener("click", () => {});
    }
}
