import { App } from "../app";
import { Event } from "../event";

declare global {
    interface HTMLElementTagNameMap {
        [AddressBar.tag]: AddressBar;
    }
}

export class AddressBar extends HTMLElement {
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
        App.addElement(element);
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
            this.url = event.detail.url;
        });

        this.addEventListener("submit", () => {
            // console.log(this.value);
            // this.set(this.value);
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

    dispatchEvent<T>(event: WebViewMessageEvent<WebViewMessageEventData<T>>) {
        return super.dispatchEvent(Event.new(event));
    }
}
