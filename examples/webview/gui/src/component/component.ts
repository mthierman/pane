import { AddressBar } from "./address_bar";
import { Button } from "./button";

export const body = document.body as HTMLBodyElement;

export const html = document.documentElement as HTMLHtmlElement;

export function getElementById<T = HTMLElement>(elementId: string) {
    return document.getElementById(elementId) as T | null;
}

export function createElement<K extends keyof HTMLElementTagNameMap>(tagName: K, id?: string) {
    const element = document.createElement(tagName);
    element.id = id || "";
    return document.createElement(tagName);
}

export function appendToBody<T extends Node>(element: T) {
    return body.appendChild(element);
}

export class Component extends HTMLElement {
    static define(name: string, constructor: CustomElementConstructor) {
        if (!customElements.get(name)) {
            customElements.define(name, constructor);
        }
    }

    dispatch<T>(event: WebViewMessageEvent<T>) {
        super.dispatchEvent(
            new CustomEvent(event.data.type, {
                detail: event.data.payload,
            }),
        );
    }
}

export { AddressBar, Button };
