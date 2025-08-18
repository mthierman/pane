import "./webview";
s
const page = {
    html: document.documentElement as HTMLHtmlElement,
    body: document.body as HTMLBodyElement,
};

export function getElementById<T = HTMLElement>(elementId: string) {
    return document.getElementById(elementId) as T | null;
}

export function createElement<K extends keyof HTMLElementTagNameMap>(tagName: K, id?: string) {
    const element = document.createElement(tagName);
    element.id = id || "";
    return document.createElement(tagName);
}

export function appendToBody<T extends Node>(element: T) {
    return page.body.appendChild(element);
}

export class Component extends HTMLElement {
    dispatch<T>(event: WebViewMessageEvent<T>) {
        super.dispatchEvent(
            new CustomEvent(event.data.type, {
                detail: event.data.payload,
            }),
        );
    }
}
