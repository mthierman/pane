import { Component } from "./component";

declare global {
    interface HTMLElementTagNameMap {
        [Button.tag]: Button;
    }
}

export class Button extends Component {
    static tag = "pane-button";

    #button = document.createElement("button") as HTMLButtonElement;

    get() {
        return this.#button;
    }

    static new(id?: string) {
        Button.define(Button.tag, this);
        let element = document.createElement(Button.tag) as Button;
        if (id) {
            element.id = id;
        }
        element.attach(element);
        return element;
    }

    connectedCallback() {
        this.appendChild(this.#button);

        this.addEventListener("click", () => {});
    }
}
