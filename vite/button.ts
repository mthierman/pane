import "./styles/index.css";

class Button extends HTMLElement {
    connectedCallback() {
        const button = document.createElement("button");
        button.textContent = "Click me";
        button.onclick = () => console.log("Button clicked!");
        this.appendChild(button);
    }
}

customElements.define("pane-button", Button);
