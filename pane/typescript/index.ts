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

    static setTitle(title: string) {
        document.title = title;
    }

    static createElement<K extends keyof HTMLElementTagNameMap>(tagName: K) {
        return document.createElement(tagName);
    }

    static getElementById<T = HTMLElement>(elementId: string) {
        return document.getElementById(elementId) as T | null;
    }
}

export class Component extends HTMLElement {
    attach<T extends Node>(parent?: T) {
        return (parent ?? Page.body).appendChild(this);
    }

    with_id(id: string) {
        this.id = id;

        return this;
    }
}

export class WebViewComponent extends Component {
    dispatchWebViewEvent<T = unknown>(event: WebViewMessageEvent<T>) {
        super.dispatchEvent(
            new CustomEvent(event.data.type, {
                detail: event.data.payload,
            }),
        );

        return this;
    }
}
