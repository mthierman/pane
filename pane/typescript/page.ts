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
