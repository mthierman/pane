export class Button extends HTMLButtonElement {
    // button = document.createElement("button");

    static define(tag = "pane-button") {
        customElements.define(tag, this, { extends: "button" });
    }

    connectedCallback() {
        // this.button.textContent = "Click me";
        // this.onclick = () => console.log("Button clicked!");
        this.onclick = () => {
            window.chrome.webview.postMessage("messagasdasde");
        };
        // this.appendChild(this.button);
    }
}

Button.define();
