export class Button extends HTMLElement {
    button = document.createElement("button");

    constructor() {
        super();
    }

    connectedCallback() {
        this.button.textContent = "Click me";
        this.button.onclick = () => console.log("Button clicked!");
        this.appendChild(this.button);
    }
}

customElements.define("pane-button", Button);
