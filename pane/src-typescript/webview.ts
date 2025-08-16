// https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/javascript/

declare global {
    interface Window {
        chrome: {
            webview: WebView;
        };
    }

    interface WebView extends EventTarget {
        hostObjects: HostObjectsAsyncRoot;
        addEventListener<T = unknown>(
            type: "message",
            listener: (event: WebViewMessageEvent<T>) => void,
            options?: boolean | AddEventListenerOptions,
        ): void;
        addEventListener<T = unknown, U = unknown>(
            type: "sharedbufferreceived",
            listener: (event: SharedBufferReceivedEvent<T, U>) => void,
            options?: boolean | AddEventListenerOptions,
        ): void;
        addEventListener(
            type: string,
            listener: EventListenerOrEventListenerObject,
            options?: boolean | AddEventListenerOptions,
        ): void;
        postMessage(message: unknown): void;
        postMessageWithAdditionalObjects(
            message: unknown,
            additionalObjects: ArrayLike<unknown>,
        ): void;
        releaseBuffer(buffer: ArrayBuffer): void;
        removeEventListener(
            type: string,
            listener: EventListenerOrEventListenerObject,
            options?: boolean | EventListenerOptions,
        ): void;
    }

    interface WebViewMessageEvent<T = unknown>
        extends MessageEvent<
            {
                [K in Extract<keyof T, string>]: { type: K; payload: T[K] };
            }[Extract<keyof T, string>]
        > {
        additionalObjects: ArrayLike<FileSystemFileHandle | FileSystemDirectoryHandle | null>;
        source: WebView & MessageEventSource;
    }

    interface SharedBufferReceivedEvent<T = unknown, U = unknown> extends Event {
        data: T;
        additionalData: U;
        source: WebView;
        getBuffer(): ArrayBuffer;
    }

    type WebViewMessageEventMap<T> = {
        [K in keyof T]: CustomEvent<T[K]>;
    };

    interface WebViewEventMap {
        message: WebViewMessageEvent;
        sharedbufferreceived: SharedBufferReceivedEvent;
    }

    interface HostObjectAsyncProxy extends HostObjectAsyncProxyBase {
        sync(): Promise<HostObjectSyncProxy>;
    }

    interface HostObjectAsyncProxyBase extends CallableFunction {
        applyHostFunction(argArray?: unknown): Promise<unknown>;
        getHostProperty(propertyName: string): Promise<unknown>;
        getLocalProperty(propertyName: string): unknown;
        setHostProperty(propertyName: string, propertyValue: unknown): Promise<unknown>;
        setLocalProperty(propertyName: string, propertyValue: unknown): unknown;
    }

    interface HostObjectsAsyncRoot extends HostObjectAsyncProxyBase {
        options: HostObjectsOptions;
        sync: HostObjectsSyncRoot;
        cancelPromise(promise: Promise<HostObjectAsyncProxy>): void;
        cleanupSome(): void;
    }

    interface HostObjectsOptions {
        defaultSyncProxy: boolean;
        forceAsyncMethodMatches: RegExp[];
        forceLocalProperties: string[];
        ignoreMemberNotFoundError: boolean;
        log: (...data: unknown[]) => void;
        shouldPassTypedArraysAsArrays: boolean;
        shouldSerializeDates: boolean;
    }

    interface HostObjectsSyncRoot extends HostObjectSyncProxy {}

    interface HostObjectSyncProxy {
        applyHostFunction(argArray?: unknown): unknown;
        async(): HostObjectAsyncProxy;
        getHostProperty(propertyName: string): unknown;
        getLocalProperty(propertyName: string): unknown;
        setHostProperty(propertyName: string, propertyValue: unknown): unknown;
        setLocalProperty(propertyName: string, propertyValue: unknown): unknown;
    }

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
