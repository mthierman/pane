declare global {
    interface CustomElementRegistry {
        get<K extends keyof HTMLElementTagNameMap>(
            name: K,
        ): (new () => HTMLElementTagNameMap[K]) | undefined;
        define<K extends keyof HTMLElementTagNameMap>(
            name: K,
            constructor: new () => HTMLElementTagNameMap[K],
            options?: ElementDefinitionOptions,
        ): void;
    }
}

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

export abstract class Component extends HTMLElement {
    dispatch<T>(event: WebViewMessageEvent<T>) {
        super.dispatchEvent(
            new CustomEvent(event.data.type, {
                detail: event.data.payload,
            }),
        );
    }
}
