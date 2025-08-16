// import type { payload } from "./index";

declare global {
    interface HTMLElementTagNameMap {
        "pane-address-bar": AddressBar;
    }

    interface HTMLElementEventMap {
        initialize: CustomEvent<string>;
    }
}

export class AddressBar extends HTMLInputElement {
    static define(tag = "pane-address-bar") {
        customElements.define(tag, this, { extends: "input" });
    }

    url = new URL("about:blank");

    constructor() {
        super();
        // console.log(this.url.href);
    }

    // addEventListener(
    //     type: "initialize",
    //     listener: (event: unknown) => void,
    //     options?: boolean | AddEventListenerOptions,
    // ): void;
    // addEventListener(
    //     type: string,
    //     listener: EventListenerOrEventListenerObject,
    //     options?: boolean | AddEventListenerOptions,
    // ): void;

    // addEventListener(
    //     type: string,
    //     listener: EventListenerOrEventListenerObject,
    //     options?: boolean | AddEventListenerOptions,
    // ): void {
    //     super.addEventListener(type, listener, options);
    // }

    connectedCallback() {
        this.addEventListener("initialize", (event) => {
            console.log(event.detail);

            this.url.href = event.detail;
            console.log(this.url.href);
            // console.log(this.url.toString());
        });
        this.addEventListener("change", () => {
            // console.log(this.value);
        });
        this.addEventListener("input", () => {
            // if (!this.value.startsWith("http")) {
            //     this.url = new URL(`https://${this.value}`);
            // } else {
            //     this.value = this.url.toString();
            // }
            // console.log(this.url);
        });
    }
}

AddressBar.define();
