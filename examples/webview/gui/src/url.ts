export class Input extends HTMLInputElement {
    static define(tag = "pane-input") {
        customElements.define(tag, this, { extends: "input" });
    }

    connectedCallback() {
        this.addEventListener("input", () => {
            if (!this.value.startsWith("http")) {
                const url = new URL(`https://${this.value}`);
                console.log(url);
            }
        });
    }
}
