import { Utility } from "../utility";

export class Component extends HTMLElement {
    static define(name: string, constructor: CustomElementConstructor) {
        if (!customElements.get(name)) {
            customElements.define(name, constructor);
        }
    }

    dispatchCustomEvent<T>(event: WebViewMessageEvent<WebViewMessageEventData<T>>) {
        super.dispatchEvent(Utility.customEvent(event));
    }
}
