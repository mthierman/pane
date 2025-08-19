import "./webview";

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

export class Page {
    static html = document.documentElement as HTMLHtmlElement;
    static head = document.head as HTMLHeadElement;
    static body = document.body as HTMLBodyElement;

    static createElement<K extends keyof HTMLElementTagNameMap>(tagName: K, id?: string) {
        const element = document.createElement(tagName);
        id ? element.setAttribute("id", id) : element.removeAttribute("id");
        return element;
    }

    static getElementById<T = HTMLElement>(elementId: string) {
        return document.getElementById(elementId) as T | null;
    }
}

export class Component extends HTMLElement {
    attach<T extends Node>(parent?: T) {
        return (parent ?? Page.body).appendChild(this);
    }

    dispatch<T = unknown>(event: WebViewMessageEvent<T>) {
        super.dispatchEvent(
            new CustomEvent(event.data.type, {
                detail: event.data.payload,
            }),
        );
    }
}
