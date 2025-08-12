class MyButton extends HTMLElement {
    constructor() {
        super();
        const shadow = this.attachShadow({ mode: "open" });
        const button = document.createElement("button");
        button.textContent = "Click me";
        button.onclick = () => {
            console.log("Button clicked!");
        };
        shadow.appendChild(button);
    }
}

customElements.define("my-button", MyButton);
