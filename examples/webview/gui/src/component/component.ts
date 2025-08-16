export class Component extends HTMLElement {
    static define(name: string, constructor: CustomElementConstructor) {
        if (!customElements.get(name)) {
            customElements.define(name, constructor);
        }
    }

    attach<T extends Node>(element: T) {
        const body = document.body as HTMLBodyElement;
        return body.appendChild(element);
    }

    dispatch<T>(event: WebViewEvent<T>) {
        super.dispatchEvent(
            new CustomEvent(event.data.type as string, {
                detail: event.data.payload,
            }),
        );
    }
}
