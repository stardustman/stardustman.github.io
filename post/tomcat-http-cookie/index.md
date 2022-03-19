
```java

package javax.servlet.http;

import java.io.Serializable;
import java.security.AccessController;
import java.security.PrivilegedAction;
import java.text.MessageFormat;
import java.util.BitSet;
import java.util.Locale;
import java.util.ResourceBundle;

/**
 * Creates a cookie, a small amount of information sent by a servlet to a Web
 * browser, saved by the browser, and later sent back to the server.
 * 创建一个 cookie， servlet 发给浏览器的一个小信息，被浏览器保存。之后再发送给服务器。
 * 
 *  A cookie's value can uniquely identify a client, so cookies are commonly used for
 * session management.
 * 一个 cookie 的值可以单独的标记出一个客户端，所以 cookies 一般被用来 session 管理。因为
 * http 是一个 stateless 的协议。
 * 
 * <p>
 * A cookie has a name, a single value, and optional attributes such as a
 * comment, path and domain qualifiers, a maximum age, and a version number.
 * 一个 cookie 有一个 name，一个单独的 value，还有一些可选的属性，比如 注释，路径，域标记
 * 最大存活时间和一个版本号。
 * document.cookie 可以得到 httponly = false 的 cookie
 *  
 * 
 * Some Web browsers have bugs in how they handle the optional attributes, so
 * use them sparingly to improve the interoperability of your servlets.
 * 一些 web browsers 在处怎样处理可选的属性有 bugs。提高 servlets 的互操作性，请慎用。
 * <p>
 * The servlet sends cookies to the browser by using the
 * {@link HttpServletResponse#addCookie} method, which adds fields to HTTP
 * response headers to send cookies to the browser, one at a time. The browser
 * is expected to support 20 cookies for each Web server, 300 cookies total, and
 * may limit cookie size to 4 KB each.
 * servlet 用 HttpServletResponse#addCookie 向 http 的相应头添加 field, 之后发送给浏览器.
 * 浏览器支持每一个 web server 保存 20 个 cookie. 每一个 cookie 的 size 可能被限制为 4 
 * 
 * <p>
 * The browser returns cookies to the servlet by adding fields to HTTP request
 * headers. 
 * 浏览器通过向 http 请求头添加 cookies 的方式返回给服务器. 
 * 
 * Cookies can be retrieved from a request by using the
 * {@link HttpServletRequest#getCookies} method. Several cookies might have the
 * same name but different path attributes.
 * 服务器通过 HttpServletRequest#getCookies 从请求中提取 cookies.
 * 一些 cookie 可能有相同的 name 但是有不同的 path attribute.
 * 
 * <p>
 * Cookies affect the caching of the Web pages that use them. HTTP 1.0 does not
 * cache pages that use cookies created with this class. This class does not
 * support the cache control defined with HTTP 1.1.
 * <p>
 * This class supports both the RFC 2109 and the RFC 6265 specifications.
 * 
 * By default, cookies are created using RFC 6265.
 * 默认的, cookie 生成方式采用 RFC 6265. 本质所有的 Cookie 都是按规范生成的.
 */
public class Cookie implements Cloneable, Serializable {

    // 校验 Cookie name
    private static final CookieNameValidator validation;

    static {
        boolean strictServletCompliance; // 严格的 servlet
        boolean strictNaming;
        boolean allowSlash;
        String propStrictNaming;
        String propFwdSlashIsSeparator;

        if (System.getSecurityManager() == null) {
            strictServletCompliance = Boolean.getBoolean(
                    "org.apache.catalina.STRICT_SERVLET_COMPLIANCE");
            propStrictNaming = System.getProperty(
                    "org.apache.tomcat.util.http.ServerCookie.STRICT_NAMING");
            propFwdSlashIsSeparator = System.getProperty(
                    "org.apache.tomcat.util.http.ServerCookie.FWD_SLASH_IS_SEPARATOR");
        } else {
            strictServletCompliance = AccessController.doPrivileged(
                    new PrivilegedAction<Boolean>() {
                        @Override
                        public Boolean run() {
                            return Boolean.valueOf(System.getProperty(
                                    "org.apache.catalina.STRICT_SERVLET_COMPLIANCE"));
                        }
                    }
                ).booleanValue();
            propStrictNaming = AccessController.doPrivileged(
                    new PrivilegedAction<String>() {
                        @Override
                        public String run() {
                            return System.getProperty(
                                    "org.apache.tomcat.util.http.ServerCookie.STRICT_NAMING");
                        }
                    }
                );
            propFwdSlashIsSeparator = AccessController.doPrivileged(
                    new PrivilegedAction<String>() {
                        @Override
                        public String run() {
                            return System.getProperty(
                                    "org.apache.tomcat.util.http.ServerCookie.FWD_SLASH_IS_SEPARATOR");
                        }
                    }
                );
        }

        if (propStrictNaming == null) {
            strictNaming = strictServletCompliance;
        } else {
            strictNaming = Boolean.parseBoolean(propStrictNaming);
        }

        if (propFwdSlashIsSeparator == null) {
            allowSlash = !strictServletCompliance;
        } else {
            allowSlash = !Boolean.parseBoolean(propFwdSlashIsSeparator);
        }

        if (strictNaming) {
            validation = new RFC2109Validator(allowSlash);
        } else {
            validation = new RFC6265Validator();
        }
    }

    private static final long serialVersionUID = 1L;

    private final String name; // cookie name 一旦创建不能被修改
    private String value; // cookie value

    private int version = 0; // ;Version=1 ... means RFC 2109 style
                             // 默认的是 RFC 6265

    //
    // Attributes encoded in the header's cookie fields.
    //
    private String comment; // ;Comment=VALUE ... describes cookie's use
    private String domain; // ;Domain=VALUE ... domain that sees cookie 可以看到 cookie 的 domain
    private int maxAge = -1; // ;Max-Age=VALUE ... cookies auto-expire 
    private String path; // ;Path=VALUE ... URLs that see the cookie 可以看到 cookie 的 urls
    private boolean secure; // ;Secure ... e.g. use SSL 启用 ssl
    private boolean httpOnly; // Not in cookie specs, but supported by browsers 不在 cookie 规范里, 但是被浏览器支持

    /**
     * Constructs a cookie with a specified name and value.
     * <p>
     * The name must conform to RFC 2109. cookie name 必须遵循 RFC 2109. 没办法,规范就是牛啊
     * That means it can contain only ASCII
     * alphanumeric characters and cannot contain commas, semicolons, or white
     * space or begin with a $ character. The cookie's name cannot be changed
     * after creation.
     * 这意味着 cookie name 只能包含 ASCII 的字母, 数字, 不能包括逗号, 分号, 空格, 或者以
     * $ 开头. cookie name 一旦创建不能被修改.
     * 
     * <p>
     * The value can be anything the server chooses to send. Its value is
     * probably of interest only to the server. The cookie's value can be
     * changed after creation with the <code>setValue</code> method.
     * server 可以通过 cookie value 发送任何东西.  
     * <p>
     * By default, cookies are created according to the Netscape cookie
     * specification. The version can be changed with the
     * <code>setVersion</code> method.
     *
     * @param name
     *            a <code>String</code> specifying the name of the cookie
     * @param value
     *            a <code>String</code> specifying the value of the cookie
     * @throws IllegalArgumentException
     *             if the cookie name contains illegal characters (for example,
     *             a comma, space, or semicolon) or it is one of the tokens
     *             reserved for use by the cookie protocol
     * @see #setValue
     * @see #setVersion
     */
    public Cookie(String name, String value) {
        // 首先进行 cookie name 的校验
        validation.validate(name);
        this.name = name;
        this.value = value;
    }

    /**
     * Specifies a comment that describes a cookie's purpose. 
     * 描述这个 cookie 的目的
     * The comment is useful if the browser presents the cookie to the user. Comments are not
     * supported by Netscape Version 0 cookies.
     *
     * @param purpose
     *            a <code>String</code> specifying the comment to display to the
     *            user
     * @see #getComment
     */
    public void setComment(String purpose) {
        comment = purpose;
    }

    /**
     * Returns the comment describing the purpose of this cookie, or
     * <code>null</code> if the cookie has no comment.
     *
     * @return a <code>String</code> containing the comment, or
     *         <code>null</code> if none
     * @see #setComment
     */
    public String getComment() {
        return comment;
    }

    /**
     * Specifies the domain within which this cookie should be presented.
     * 指定这个 cookie 可以出现在那个 domain
     * <p>
     * The form of the domain name is specified by RFC 2109. A domain name
     * begins with a dot (<code>.foo.com</code>) and means that the cookie is
     * visible to servers in a specified Domain Name System (DNS) zone (for
     * example, <code>www.foo.com</code>, but not <code>a.b.foo.com</code>). By
     * default, cookies are only returned to the server that sent them.
     * 
     * domain name 的形式说明在 RFC 2109. domain name 以 dot 开头, 这意味着在一个特定的
     * DNS 这个 cookie 是可见的. 默认的, 那个服务器发送的 cookie 返回给哪个服务器.
     *
     * @param pattern
     *            a <code>String</code> containing the domain name within which
     *            this cookie is visible; form is according to RFC 2109
     * @see #getDomain
     */
    public void setDomain(String pattern) {
        domain = pattern.toLowerCase(Locale.ENGLISH); // IE allegedly needs this
    }

    /**
     * Returns the domain name set for this cookie. The form of the domain name
     * is set by RFC 2109.
     *
     * @return a <code>String</code> containing the domain name
     * @see #setDomain
     */
    public String getDomain() {
        return domain;
    }

    /**
     * Sets the maximum age of the cookie in seconds.
     * cookie 最大存活的秒数
     * <p>
     * A positive value indicates that the cookie will expire after that many
     * seconds have passed. Note that the value is the <i>maximum</i> age when
     * the cookie will expire, not the cookie's current age.
     * 一个正数表明 cookie 将在 maximun age 秒数后过期.
     * <p>
     * A negative value means that the cookie is not stored persistently and
     * will be deleted when the Web browser exits. A zero value causes the
     * cookie to be deleted.
     * 
     * 一个负数表示 cookie 不会被永久存储, 浏览器退出时将被删除.
     * 0  删除这个 cookie
     *
     * @param expiry
     *            an integer specifying the maximum age of the cookie in
     *            seconds; if negative, means the cookie is not stored; if zero,
     *            deletes the cookie
     * @see #getMaxAge
     */
    public void setMaxAge(int expiry) {
        maxAge = expiry;
    }

    /**
     * Returns the maximum age of the cookie, specified in seconds, By default,
     * <code>-1</code> indicating the cookie will persist until browser
     * shutdown.
     *
     * @return an integer specifying the maximum age of the cookie in seconds; if
     *         negative, means the cookie persists until browser shutdown
     * @see #setMaxAge
     */
    public int getMaxAge() {
        return maxAge;
    }

    /**
     * Specifies a path for the cookie to which the client should return the
     * cookie.
     * 指定一个 path, 表示哪一个 client 应该把这个 cookie 返还给服务器.
     * 
     * <p>
     * The cookie is visible to all the pages in the directory you specify, and
     * all the pages in that directory's subdirectories. A cookie's path must
     * include the servlet that set the cookie, for example, <i>/catalog</i>,
     * which makes the cookie visible to all directories on the server under
     * <i>/catalog</i>.
     * <p>
     * Consult RFC 2109 (available on the Internet) for more information on
     * setting path names for cookies.
     *
     * @param uri
     *            a <code>String</code> specifying a path
     * @see #getPath
     */
    public void setPath(String uri) {
        path = uri;
    }

    /**
     * Returns the path on the server to which the browser returns this cookie.
     * The cookie is visible to all subpaths on the server.
     *
     * @return a <code>String</code> specifying a path that contains a servlet
     *         name, for example, <i>/catalog</i>
     * @see #setPath
     */
    public String getPath() {
        return path;
    }

    /**
     * Indicates to the browser whether the cookie should only be sent using a
     * secure protocol, such as HTTPS or SSL.
     * <p>
     * The default value is <code>false</code>.
     *
     * @param flag
     *            if <code>true</code>, sends the cookie from the browser to the
     *            server only when using a secure protocol; if
     *            <code>false</code>, sent on any protocol
     * @see #getSecure
     */
    public void setSecure(boolean flag) {
        secure = flag;
    }

    /**
     * Returns <code>true</code> if the browser is sending cookies only over a
     * secure protocol, or <code>false</code> if the browser can send cookies
     * using any protocol.
     *
     * @return <code>true</code> if the browser uses a secure protocol;
     *         otherwise, <code>true</code>
     * @see #setSecure
     */
    public boolean getSecure() {
        return secure;
    }

    /**
     * Returns the name of the cookie. The name cannot be changed after
     * creation.
     *
     * @return a <code>String</code> specifying the cookie's name
     */
    public String getName() {
        return name;
    }

    /**
     * Assigns a new value to a cookie after the cookie is created. If you use a
     * binary value, you may want to use BASE64 encoding.
     * cookie 被创建后, 给 value 指定一个值. 如果使用二进制的值, 采用 BASE64 的编码方式.
     * 
     * <p>
     * With Version 0 cookies, values should not contain white space, brackets,
     * parentheses, equals signs, commas, double quotes, slashes, question
     * marks, at signs, colons, and semicolons. Empty values may not behave the
     * same way on all browsers.
     *
     * @param newValue
     *            a <code>String</code> specifying the new value
     * @see #getValue
     * @see Cookie
     */
    public void setValue(String newValue) {
        value = newValue;
    }

    /**
     * Returns the value of the cookie.
     *
     * @return a <code>String</code> containing the cookie's present value
     * @see #setValue
     * @see Cookie
     */
    public String getValue() {
        return value;
    }

    /**
     * Returns the version of the protocol this cookie complies with. Version 1
     * complies with RFC 2109, and version 0 complies with the original cookie
     * specification drafted by Netscape. Cookies provided by a browser use and
     * identify the browser's cookie version.
     *
     * @return 0 if the cookie complies with the original Netscape specification;
     *         1 if the cookie complies with RFC 2109  被 RFC 6265 淘汰
     * @see #setVersion
     */
    public int getVersion() {
        return version;
    }

    /**
     * Sets the version of the cookie protocol this cookie complies with.
     * Version 0 complies with the original Netscape cookie specification.
     * version 0 遵守原始的 netscape cookie 说明
     * Version 1 complies with RFC 2109.
     * version 1 遵守 RFC 2109 规范
     * <p>
     * Since RFC 2109 is still somewhat new, consider version 1 as experimental;
     * do not use it yet on production sites.
     *
     * @param v
     *            0 if the cookie should comply with the original Netscape
     *            specification; 1 if the cookie should comply with RFC 2109
     * @see #getVersion
     */
    public void setVersion(int v) {
        version = v;
    }

    /**
     * Overrides the standard <code>java.lang.Object.clone</code> method to
     * return a copy of this cookie.
     */
    @Override
    public Object clone() {
        try {
            return super.clone();
        } catch (CloneNotSupportedException e) {
            throw new RuntimeException(e);
        }
    }

    /**
     * Sets the flag that controls if this cookie will be hidden from scripts on
     * the client side.
     * 
     * true 不能被 js document.cookie 操作
     *
     * @param httpOnly  The new value of the flag
     *
     * @since Servlet 3.0
     */
    public void setHttpOnly(boolean httpOnly) {
        this.httpOnly = httpOnly;
    }

    /**
     * Gets the flag that controls if this cookie will be hidden from scripts on
     * the client side.
     *
     * @return  <code>true</code> if the cookie is hidden from scripts, else
     *          <code>false</code>
     * @since Servlet 3.0
     */
    public boolean isHttpOnly() {
        return httpOnly;
    }
}


class CookieNameValidator {

    // 拿到 properties 文件的方法 ResourceBundle
    private static final String LSTRING_FILE = "javax.servlet.http.LocalStrings";
    protected static final ResourceBundle lStrings = ResourceBundle.getBundle(LSTRING_FILE);

    protected final BitSet allowed;

    protected CookieNameValidator(String separators) {
        allowed = new BitSet(128);
        allowed.set(0x20, 0x7f); // any CHAR except CTLs or separators
        for (int i = 0; i < separators.length(); i++) {
            char ch = separators.charAt(i);
            allowed.clear(ch);
        }
    }

    void validate(String name) {
        // cookie name 不允许为 null 或者长度为 0
        if (name == null || name.length() == 0) {
            throw new IllegalArgumentException(lStrings.getString("err.cookie_name_blank"));
        }
        if (!isToken(name)) {
            String errMsg = lStrings.getString("err.cookie_name_is_token");
            throw new IllegalArgumentException(MessageFormat.format(errMsg, name));
        }
    }

    private boolean isToken(String possibleToken) {
        int len = possibleToken.length();

        for (int i = 0; i < len; i++) {
            char c = possibleToken.charAt(i);
            if (!allowed.get(c)) {
                return false;
            }
        }
        return true;
    }
}

class RFC6265Validator extends CookieNameValidator {
    private static final String RFC2616_SEPARATORS = "()<>@,;:\\\"/[]?={} \t";

    RFC6265Validator() {
        super(RFC2616_SEPARATORS);
    }
}

class RFC2109Validator extends RFC6265Validator {
    RFC2109Validator(boolean allowSlash) {
        // special treatment to allow for FWD_SLASH_IS_SEPARATOR property
        if (allowSlash) {
            allowed.set('/');
        }
    }

    @Override
    void validate(String name) {
        super.validate(name);
        // cookie name 不允许 $ 开头
        if (name.charAt(0) == '$') {
            String errMsg = lStrings.getString("err.cookie_name_is_token");
            throw new IllegalArgumentException(MessageFormat.format(errMsg, name));
        }
    }
}
```
