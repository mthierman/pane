export class Button extends HTMLElement {
    button = document.createElement("button");

    static define(tag = "pane-button") {
        customElements.define(tag, this);
    }

    connectedCallback() {
        this.button.textContent = "Click me";
        this.button.onclick = () => console.log("Button clicked!");
        this.appendChild(this.button);
    }
}

Button.define();
