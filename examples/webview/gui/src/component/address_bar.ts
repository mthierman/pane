declare global {
    interface HTMLElementTagNameMap {
        "pane-address-bar": AddressBar;
    }
}

export class AddressBar extends HTMLInputElement {
    static tag = "pane-address-bar";
    static define() {
        customElements.define(AddressBar.tag, this, { extends: "input" });
    }

    url = new URL("about:blank");

    set(href: string) {
        this.url.href = href;
        this.value = this.url.href;
    }

    connectedCallback() {
        this.type = "text";
        // this.setAttribute("is", "pane-address-bar");
        this.addEventListener("initialize", (event) => {
            this.set(event.detail.url);
        });

        this.addEventListener("submit", () => {
            console.log(this.value);
            this.set(this.value);

            // if (!this.value.startsWith("http")) {
            //     console.log("!http");
            //     this.url = new URL(`https://${this.value}`);
            // } else {
            //     console.log(new URL(this.value));
            //     this.value = this.url.toString();
            // }

            // if (!this.value.startsWith("http")) {
            //     this.url = new URL(`https://${this.value}`);
            // } else {
            //     this.value = this.url.toString();
            // }
            // console.log(this.url);
        });
    }
}
