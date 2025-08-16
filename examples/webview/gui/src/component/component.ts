export const body = document.body as HTMLBodyElement;

export const html = document.documentElement as HTMLHtmlElement;

export class Component extends HTMLElement {
    static define(name: string, constructor: CustomElementConstructor) {
        if (!customElements.get(name)) {
            customElements.define(name, constructor);
        }
    }

    attach<T extends Node>(element: T) {
        return body.appendChild(element);
    }

    dispatch<T>(event: WebViewMessageEvent<WebViewMessageEventData<T>>) {
        super.dispatchEvent(
            new CustomEvent(event.data.type as string, {
                detail: event.data.payload,
            }),
        );
    }
}
