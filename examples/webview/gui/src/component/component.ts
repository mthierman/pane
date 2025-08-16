import { Utility } from "../utility";

export class Component extends HTMLElement {
    dispatchCustomEvent<T>(event: WebViewMessageEvent<WebViewMessageEventData<T>>) {
        super.dispatchEvent(Utility.customEvent(event));
    }
}
