import { Utility } from "../utility";
import { Component } from "./component";

declare global {
    interface HTMLElementTagNameMap {
        [AddressBar.tag]: AddressBar;
    }
}

export class AddressBar extends Component {
    static tag = "pane-address-bar";

    static define() {
        if (!customElements.get(AddressBar.tag)) {
            customElements.define(AddressBar.tag, this);
        }
    }

    #url = new URL("about:blank");
    input = document.createElement("input") as HTMLInputElement;

    static new() {
        AddressBar.define();
        let element = document.createElement(AddressBar.tag) as AddressBar;
        Utility.addElement(element);
        return element;
    }

    get url(): URL {
        return this.#url;
    }

    set url(href: string) {
        this.#url.href = href;
        this.input.value = this.#url.href;
    }

    connectedCallback() {
        this.appendChild(this.input);

        this.addEventListener("init", (event) => {
            console.log("init");
            this.url = event.detail.url;
        });

        this.addEventListener("keydown", (event) => {
            // console.log(`keydown ${event.key}`);
            // if (event.key === "Enter") {
            //     const test: WebViewMessageEventData<payload> = {
            //         type: "init",
            //         payload: {
            //             url: this.url.href,
            //         },
            //     };
            //     window.chrome.webview.postMessage(test);
            // }
            // this.dispatchEvent();
        });

        this.addEventListener("submit", () => {
            if (!this.input.value.startsWith("http")) {
                this.url = `https://${this.input.value}`;
                console.log(this.url);
            } else {
                this.input.value = this.url.toString();
            }
        });
    }
}
