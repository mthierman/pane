export class Input extends HTMLInputElement {
    static define(tag = "pane-input") {
        customElements.define(tag, this, { extends: "input" });
    }

    connectedCallback() {
        this.addEventListener("input", () => {
            console.log(this.value);
        });
    }
}
