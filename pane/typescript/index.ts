import "./webview";

export class Page {
    static html() {
        return document.documentElement as HTMLHtmlElement;
    }

    static head() {
        return document.head as HTMLHeadElement;
    }

    static body() {
        return document.body as HTMLBodyElement;
    }

    static appendToBody<T extends Node>(element: T) {
        return Page.body().appendChild(element);
    }

    static getElementById<T = HTMLElement>(elementId: string) {
        return document.getElementById(elementId) as T | null;
    }

    static createElement<K extends keyof HTMLElementTagNameMap>(tagName: K, id?: string) {
        const element = document.createElement(tagName);
        element.id = id || "";
        return document.createElement(tagName);
    }
}

export class Component extends HTMLElement {
    append<T extends Node>(parent: T) {
        return parent.appendChild(this);
    }

    dispatch<T>(event: WebViewMessageEvent<T>) {
        super.dispatchEvent(
            new CustomEvent(event.data.type, {
                detail: event.data.payload,
            }),
        );
    }
}
