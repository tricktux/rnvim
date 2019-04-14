/*
 * =====================================================================================
 *
 *       Filename: libnvc.hpp
 *        Created: 12/27/2018 04:52:17
 *    Description: 
 *
 *        Version: 1.0
 *       Revision: none
 *       Compiler: gcc
 *
 *         Author: ANHONG
 *          Email: anhonghe@gmail.com
 *   Organization: USTC
 *
 * =====================================================================================
 */

#pragma once

#include <map>
#include <vector>
#include <memory>
#include <variant>
#include <cstring>
#include <cstdint>
#include <cstddef>
#include <functional>
#include <type_traits>
#include <unordered_map>

namespace libnvc
{
    constexpr int LOG_INFO    = 0;
    constexpr int LOG_WARNING = 1;
    constexpr int LOG_FATAL   = 2;
    constexpr int LOG_DEBUG   = 3;

    void log(int, const char *);
    void set_log(std::function<void(int, const char *)>);
}

namespace libnvc::ctf
{
    constexpr size_t strlen(const char *str)
    {
        size_t index = 0;
        while(str[index] != '\0'){
            ++index;
        }
        return index;
    }

    constexpr int strcmp(const char *str1, const char *str2)
    {
        for(size_t index = 0;; ++index){
            if(str1[index] < str2[index]){
                return -1;
            }

            if(str1[index] > str2[index]){
                return 1;
            }

            if(str1[index] == '\0'){
                return 0;
            }
        }
    }

    constexpr int strncmp(const char *str1, const char *str2, size_t size)
    {
        for(size_t index = 0; index < size; ++index){
            if(str1[index] < str2[index]){
                return -1;
            }

            if(str1[index] > str2[index]){
                return 1;
            }

            if(str1[index] == '\0'){
                return 0;
            }
        }
        return 0;
    }

    constexpr int chmemcmp(const char *mem1, const char *mem2, size_t size)
    {
        for(size_t index = 0; index < size; ++index){
            if(mem1[index] < mem2[index]){
                return -1;
            }

            if(mem1[index] > mem2[index]){
                return +1;
            }
        }
        return 0;
    }

    constexpr void chmemset(char *dst, char value, size_t size)
    {
        for(size_t index = 0; index < size; ++index){
            dst[index] = value;
        }
    }

    constexpr char *strcat(char *dst, const char *src)
    {
        size_t index  = 0;
        size_t length = libnvc::ctf::strlen(dst);

        for(index = 0; src[index] != '\0'; ++index){
            dst[length + index] = src[index];
        }

        dst[length + index] = '\0';
        return dst;
    }
}

namespace libnvc
{
    constexpr const char * g_nvim_api_reserved_string_tbl[]
    {
        // put the zero index as reserved
        // only allow the first entry as nullptr
        nullptr,

        // requests
        "req::nvim_buf_line_count",
        "req::nvim_buf_attach",
        "req::nvim_buf_detach",
        "req::nvim_buf_get_lines",
        "req::nvim_buf_set_lines",
        "req::nvim_buf_get_offset",
        "req::nvim_buf_get_var",
        "req::nvim_buf_get_changedtick",
        "req::nvim_buf_get_keymap",
        "req::nvim_buf_get_commands",
        "req::nvim_buf_set_var",
        "req::nvim_buf_del_var",
        "req::nvim_buf_get_option",
        "req::nvim_buf_set_option",
        "req::nvim_buf_get_name",
        "req::nvim_buf_set_name",
        "req::nvim_buf_is_loaded",
        "req::nvim_buf_is_valid",
        "req::nvim_buf_get_mark",
        "req::nvim_buf_add_highlight",
        "req::nvim_buf_clear_namespace",
        "req::nvim_buf_clear_highlight",
        "req::nvim_buf_set_virtual_text",
        "req::nvim_tabpage_list_wins",
        "req::nvim_tabpage_get_var",
        "req::nvim_tabpage_set_var",
        "req::nvim_tabpage_del_var",
        "req::nvim_tabpage_get_win",
        "req::nvim_tabpage_get_number",
        "req::nvim_tabpage_is_valid",
        "req::nvim_ui_attach",
        "req::nvim_ui_detach",
        "req::nvim_ui_try_resize",
        "req::nvim_ui_set_option",
        "req::nvim_ui_try_resize_grid",
        "req::nvim_command",
        "req::nvim_get_hl_by_name",
        "req::nvim_get_hl_by_id",
        "req::nvim_feedkeys",
        "req::nvim_input",
        "req::nvim_input_mouse",
        "req::nvim_replace_termcodes",
        "req::nvim_command_output",
        "req::nvim_eval",
        "req::nvim_execute_lua",
        "req::nvim_call_function",
        "req::nvim_call_dict_function",
        "req::nvim_strwidth",
        "req::nvim_list_runtime_paths",
        "req::nvim_set_current_dir",
        "req::nvim_get_current_line",
        "req::nvim_set_current_line",
        "req::nvim_del_current_line",
        "req::nvim_get_var",
        "req::nvim_set_var",
        "req::nvim_del_var",
        "req::nvim_get_vvar",
        "req::nvim_set_vvar",
        "req::nvim_get_option",
        "req::nvim_set_option",
        "req::nvim_out_write",
        "req::nvim_err_write",
        "req::nvim_err_writeln",
        "req::nvim_list_bufs",
        "req::nvim_get_current_buf",
        "req::nvim_set_current_buf",
        "req::nvim_list_wins",
        "req::nvim_get_current_win",
        "req::nvim_set_current_win",
        "req::nvim_create_buf",
        "req::nvim_open_win",
        "req::nvim_list_tabpages",
        "req::nvim_get_current_tabpage",
        "req::nvim_set_current_tabpage",
        "req::nvim_create_namespace",
        "req::nvim_get_namespaces",
        "req::nvim_subscribe",
        "req::nvim_unsubscribe",
        "req::nvim_get_color_by_name",
        "req::nvim_get_color_map",
        "req::nvim_get_mode",
        "req::nvim_get_keymap",
        "req::nvim_get_commands",
        "req::nvim_get_api_info",
        "req::nvim_set_client_info",
        "req::nvim_get_chan_info",
        "req::nvim_list_chans",
        "req::nvim_call_atomic",
        "req::nvim_parse_expression",
        "req::nvim_list_uis",
        "req::nvim_get_proc_children",
        "req::nvim_get_proc",
        "req::nvim_select_popupmenu_item",
        "req::nvim_win_get_buf",
        "req::nvim_win_set_buf",
        "req::nvim_win_get_cursor",
        "req::nvim_win_set_cursor",
        "req::nvim_win_get_height",
        "req::nvim_win_set_height",
        "req::nvim_win_get_width",
        "req::nvim_win_set_width",
        "req::nvim_win_get_var",
        "req::nvim_win_set_var",
        "req::nvim_win_del_var",
        "req::nvim_win_get_option",
        "req::nvim_win_set_option",
        "req::nvim_win_get_position",
        "req::nvim_win_get_tabpage",
        "req::nvim_win_get_number",
        "req::nvim_win_is_valid",
        "req::nvim_win_set_config",
        "req::nvim_win_get_config",
        "req::nvim_win_close",

        // notifications
        "notif::test",
    };

    constexpr const char *idstr(size_t id)
    {
        if(id == 0 || id >= std::extent<decltype(g_nvim_api_reserved_string_tbl)>::value){
            return nullptr;
        }
        return g_nvim_api_reserved_string_tbl[id];
    }

    constexpr const char *reqstr(size_t id)
    {
        if(id == 0 || id >= std::extent<decltype(g_nvim_api_reserved_string_tbl)>::value){
            return nullptr;
        }

        if(auto str = g_nvim_api_reserved_string_tbl[id]; libnvc::ctf::strncmp(str, "req::", 5) == 0){
            return str + 5;
        }
        return nullptr;
    }

    constexpr size_t strid(const char *str)
    {
        if(str == nullptr){
            return 0;
        }

        for(size_t index = 1; index < std::extent<decltype(g_nvim_api_reserved_string_tbl)>::value; ++index){
            if(libnvc::ctf::strcmp(str, g_nvim_api_reserved_string_tbl[index]) == 0){
                return index;
            }
        }
        return 0;
    }

    constexpr size_t strid(const char *prefix, const char *name)
    {
        char buf[512] = {'\0'};
        libnvc::ctf::strcat(buf, prefix);
        libnvc::ctf::strcat(buf, "::"  );
        libnvc::ctf::strcat(buf, name  );
        return libnvc::strid(buf);
    }

    constexpr size_t reqid(const char *req_name)
    {
        return strid("req", req_name);
    }

    constexpr size_t notifid(const char *notif_name)
    {
        return strid("notif", notif_name);
    }

    constexpr size_t min_reqid()
    {
        for(size_t id = 1; libnvc::idstr(id) != nullptr; ++id){
            if(libnvc::ctf::strncmp(libnvc::idstr(id), "req::", 5) == 0){
                return id;
            }
        }
        return 0;
    }

    constexpr size_t max_reqid()
    {
        for(size_t id = std::extent<decltype(g_nvim_api_reserved_string_tbl)>::value - 1; libnvc::idstr(id) != nullptr; --id){
            if(libnvc::ctf::strncmp(libnvc::idstr(id), "req::", 5) == 0){
                return id;
            }
        }
        return 0;
    }

    constexpr size_t min_notifid()
    {
        for(size_t id = 1; libnvc::idstr(id) != nullptr; ++id){
            if(libnvc::ctf::strncmp(libnvc::idstr(id), "notif::", 7) == 0){
                return id;
            }
        }
        return 0;
    }

    constexpr size_t max_notifid()
    {
        for(size_t id = std::extent<decltype(g_nvim_api_reserved_string_tbl)>::value - 1; libnvc::idstr(id) != nullptr; --id){
            if(libnvc::ctf::strncmp(libnvc::idstr(id), "notif::", 7) == 0){
                return id;
            }
        }
        return 0;
    }
}

namespace libnvc
{
    class io_device
    {
        public:
            io_device() = default;

        public:
            virtual ~io_device() = default;

        public:
            virtual size_t send(const char *, size_t) = 0; // blocking send, return after done all bytes
            virtual size_t recv(      char *, size_t) = 0;

        public:
            size_t send(std::string_view s)
            {
                return send(s.data(), s.length());
            }
    };
}

namespace libnvc
{
    class socket: public libnvc::io_device
    {
        public:
            socket() = default;

        public:
            virtual ~socket() = default;

        public:
            virtual bool connect(const char *, int) = 0;
            virtual void disconnect()               = 0;
    };

    class asio_socket: public libnvc::socket
    {
        private:
            class asio_socket_impl;

        private:
            std::unique_ptr<asio_socket_impl> m_impl;

        public:
            asio_socket();

        public:
            virtual ~asio_socket();

        public:
            virtual bool connect(const char *, int);
            virtual void disconnect();

        public:
            virtual size_t send(const char *, size_t);
            virtual size_t recv(      char *, size_t);
    };
}

namespace libnvc
{
    constexpr int64_t REQ   = 0;
    constexpr int64_t RESP  = 1;
    constexpr int64_t NOTIF = 2;
}

namespace libnvc
{
    struct _void_type
    {
        int unused = 0;
    };
    using void_type = struct _void_type;


    // TODO: need a better implementation
    // using helper class object_wrapper since  we can't define recursive type in cpp

    // be careful of std::vector<object> vs std::vector<object_wrapper>
    // this can easily give bug
    class object_wrapper;
    using object = std::variant<
          bool,
          int64_t,
          double,
          std::string,
          std::vector<libnvc::object_wrapper>,
          std::map<std::string, libnvc::object_wrapper>
    >;

    class object_wrapper
    {
        public:
            object_wrapper() = default;

        public:
            template<typename T> object_wrapper(T t)
            {
                m_ptr = std::make_shared<libnvc::object>(t);
            }

        private:
            // use shared_ptr not unique_ptr
            // the sematics of object_wrapper is a wrapper of an existing object
            std::shared_ptr<libnvc::object> m_ptr;

            // don't define operator object ()
            // since which returns a copy of the included object

        public:
            libnvc::object &ref()
            {
                return *(m_ptr.get());
            }

            const libnvc::object &ref() const
            {
                return *(m_ptr.get());
            }
    };

    using resp_variant = std::variant<libnvc::void_type,
            int64_t, 
                bool, 
                std::vector<std::string>, 
                    libnvc::object, 
                std::vector<std::map<std::string, libnvc::object>>, 
                std::map<std::string, libnvc::object>, 
                std::string, 
                std::array<int64_t, 2>, 
                std::vector<int64_t>, 
                std::vector<libnvc::object> 
        >;
}

namespace libnvc
{
    inline int64_t msgid(size_t req_id, int64_t seq_id)
    {
        // put seq_id at high bits
        // this helps to make the key/value pairs sorted by sent time
        return ((seq_id & 0x0000ffffffffffff) << 16) | ((int64_t)(req_id) & 0x000000000000ffff);
    }

    inline std::tuple<size_t, int64_t> msgid_decomp(int64_t msg_id)
    {
        return {(size_t)(msg_id & 0x000000000000ffff), msg_id >> 16};
    }
}

namespace libnvc::mpinterf
{
    class writer
    {
        private:
            constexpr static size_t m_writer_size = []()
            {
#ifdef LIBNVC_MPACK_WRITER_SIZE
                return LIBNVC_MPACK_WRITER_SIZE;
#else
                return 128;
#endif
            }();

            constexpr static size_t m_writer_align = []()
            {
#ifdef LIBNVC_MPACK_WRITER_ALIGN
                return LIBNVC_MPACK_WRITER_ALIGN;
#else
                return 16;
#endif
            }();

        private:
            std::aligned_storage_t<m_writer_size, m_writer_align> m_storage;

        private:
            // need this helper variable to record if writer alive
            // there is no official way to track if the writer called mpack_writer_destroy()
            bool m_writer_alive;

        private:
            char  *m_data;
            size_t m_size;

        public:
            writer();

        public:
            ~writer();

        private:
            void *storage()
            {
                return &m_storage;
            }

        private:
            void flush();

        private:
            void clear();
            void reset();

        private:
            std::string_view pack();

        private:
            template<typename T, typename... U> void write(T t, U... u)
            {
                write(t);
                if(sizeof...(u) != 0){
                    write(u...);
                }
            }

        private:
            void write();
            void write(bool);
            void write(double);
            void write(int64_t);
            void write(const char *);
            void write(const std::string&);
            void write(const libnvc::object&);
            void write(const std::array<int64_t, 2> &);
            void write(const std::map<std::string, libnvc::object>&);

        private:
            void start_array(size_t);
            void finish_array();

        public:
            template<typename... Args> std::string_view pack_req(int64_t msgid, Args... args)
            {
                auto [req_id, seq_id] = libnvc::msgid_decomp(msgid);
                if(seq_id <= 0){
                }

                reset();
                start_array(4);
                {
                    write(libnvc::REQ);
                    write(msgid);
                    write(libnvc::reqstr(req_id));
                    start_array(sizeof...(args));
                    {
                        write(args...);
                    }
                    finish_array();

                }
                finish_array();
                return pack();
            }
    };
}

namespace libnvc
{
    template<size_t> struct req;

    template<> struct req<libnvc::reqid("nvim_buf_line_count")>
    {
        using parms_tuple = std::tuple<
            int64_t 
        >;

        using resp_type = int64_t;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_buf_line_count");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_buf_attach")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            bool, 
            std::map<std::string, libnvc::object> 
        >;

        using resp_type = bool;

        constexpr int since() const
        {
            return 4;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_buf_attach");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_buf_detach")>
    {
        using parms_tuple = std::tuple<
            int64_t 
        >;

        using resp_type = bool;

        constexpr int since() const
        {
            return 4;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_buf_detach");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_buf_get_lines")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            int64_t, 
            int64_t, 
            bool 
        >;

        using resp_type = std::vector<std::string>;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_buf_get_lines");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_buf_set_lines")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            int64_t, 
            int64_t, 
            bool, 
            std::vector<std::string> 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_buf_set_lines");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_buf_get_offset")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            int64_t 
        >;

        using resp_type = int64_t;

        constexpr int since() const
        {
            return 5;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_buf_get_offset");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_buf_get_var")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            std::string 
        >;

        using resp_type = libnvc::object;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_buf_get_var");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_buf_get_changedtick")>
    {
        using parms_tuple = std::tuple<
            int64_t 
        >;

        using resp_type = int64_t;

        constexpr int since() const
        {
            return 2;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_buf_get_changedtick");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_buf_get_keymap")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            std::string 
        >;

        using resp_type = std::vector<std::map<std::string, libnvc::object>>;

        constexpr int since() const
        {
            return 3;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_buf_get_keymap");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_buf_get_commands")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            std::map<std::string, libnvc::object> 
        >;

        using resp_type = std::map<std::string, libnvc::object>;

        constexpr int since() const
        {
            return 4;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_buf_get_commands");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_buf_set_var")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            std::string, 
            libnvc::object 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_buf_set_var");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_buf_del_var")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            std::string 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_buf_del_var");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_buf_get_option")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            std::string 
        >;

        using resp_type = libnvc::object;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_buf_get_option");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_buf_set_option")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            std::string, 
            libnvc::object 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_buf_set_option");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_buf_get_name")>
    {
        using parms_tuple = std::tuple<
            int64_t 
        >;

        using resp_type = std::string;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_buf_get_name");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_buf_set_name")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            std::string 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_buf_set_name");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_buf_is_loaded")>
    {
        using parms_tuple = std::tuple<
            int64_t 
        >;

        using resp_type = bool;

        constexpr int since() const
        {
            return 5;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_buf_is_loaded");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_buf_is_valid")>
    {
        using parms_tuple = std::tuple<
            int64_t 
        >;

        using resp_type = bool;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_buf_is_valid");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_buf_get_mark")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            std::string 
        >;

        using resp_type = std::array<int64_t, 2>;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_buf_get_mark");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_buf_add_highlight")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            int64_t, 
            std::string, 
            int64_t, 
            int64_t, 
            int64_t 
        >;

        using resp_type = int64_t;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_buf_add_highlight");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_buf_clear_namespace")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            int64_t, 
            int64_t, 
            int64_t 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 5;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_buf_clear_namespace");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_buf_clear_highlight")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            int64_t, 
            int64_t, 
            int64_t 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_buf_clear_highlight");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_buf_set_virtual_text")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            int64_t, 
            int64_t, 
            std::vector<libnvc::object>, 
            std::map<std::string, libnvc::object> 
        >;

        using resp_type = int64_t;

        constexpr int since() const
        {
            return 5;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_buf_set_virtual_text");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_tabpage_list_wins")>
    {
        using parms_tuple = std::tuple<
            int64_t 
        >;

        using resp_type = std::vector<int64_t>;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_tabpage_list_wins");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_tabpage_get_var")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            std::string 
        >;

        using resp_type = libnvc::object;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_tabpage_get_var");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_tabpage_set_var")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            std::string, 
            libnvc::object 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_tabpage_set_var");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_tabpage_del_var")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            std::string 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_tabpage_del_var");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_tabpage_get_win")>
    {
        using parms_tuple = std::tuple<
            int64_t 
        >;

        using resp_type = int64_t;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_tabpage_get_win");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_tabpage_get_number")>
    {
        using parms_tuple = std::tuple<
            int64_t 
        >;

        using resp_type = int64_t;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_tabpage_get_number");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_tabpage_is_valid")>
    {
        using parms_tuple = std::tuple<
            int64_t 
        >;

        using resp_type = bool;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_tabpage_is_valid");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_ui_attach")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            int64_t, 
            std::map<std::string, libnvc::object> 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_ui_attach");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_ui_detach")>
    {
        using parms_tuple = std::tuple<
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_ui_detach");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_ui_try_resize")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            int64_t 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_ui_try_resize");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_ui_set_option")>
    {
        using parms_tuple = std::tuple<
            std::string, 
            libnvc::object 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_ui_set_option");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_ui_try_resize_grid")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            int64_t, 
            int64_t 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 6;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_ui_try_resize_grid");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_command")>
    {
        using parms_tuple = std::tuple<
            std::string 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_command");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_get_hl_by_name")>
    {
        using parms_tuple = std::tuple<
            std::string, 
            bool 
        >;

        using resp_type = std::map<std::string, libnvc::object>;

        constexpr int since() const
        {
            return 3;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_get_hl_by_name");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_get_hl_by_id")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            bool 
        >;

        using resp_type = std::map<std::string, libnvc::object>;

        constexpr int since() const
        {
            return 3;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_get_hl_by_id");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_feedkeys")>
    {
        using parms_tuple = std::tuple<
            std::string, 
            std::string, 
            bool 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_feedkeys");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_input")>
    {
        using parms_tuple = std::tuple<
            std::string 
        >;

        using resp_type = int64_t;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_input");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_input_mouse")>
    {
        using parms_tuple = std::tuple<
            std::string, 
            std::string, 
            std::string, 
            int64_t, 
            int64_t, 
            int64_t 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 6;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_input_mouse");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_replace_termcodes")>
    {
        using parms_tuple = std::tuple<
            std::string, 
            bool, 
            bool, 
            bool 
        >;

        using resp_type = std::string;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_replace_termcodes");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_command_output")>
    {
        using parms_tuple = std::tuple<
            std::string 
        >;

        using resp_type = std::string;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_command_output");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_eval")>
    {
        using parms_tuple = std::tuple<
            std::string 
        >;

        using resp_type = libnvc::object;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_eval");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_execute_lua")>
    {
        using parms_tuple = std::tuple<
            std::string, 
            std::vector<libnvc::object> 
        >;

        using resp_type = libnvc::object;

        constexpr int since() const
        {
            return 3;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_execute_lua");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_call_function")>
    {
        using parms_tuple = std::tuple<
            std::string, 
            std::vector<libnvc::object> 
        >;

        using resp_type = libnvc::object;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_call_function");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_call_dict_function")>
    {
        using parms_tuple = std::tuple<
            libnvc::object, 
            std::string, 
            std::vector<libnvc::object> 
        >;

        using resp_type = libnvc::object;

        constexpr int since() const
        {
            return 4;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_call_dict_function");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_strwidth")>
    {
        using parms_tuple = std::tuple<
            std::string 
        >;

        using resp_type = int64_t;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_strwidth");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_list_runtime_paths")>
    {
        using parms_tuple = std::tuple<
        >;

        using resp_type = std::vector<std::string>;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_list_runtime_paths");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_set_current_dir")>
    {
        using parms_tuple = std::tuple<
            std::string 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_set_current_dir");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_get_current_line")>
    {
        using parms_tuple = std::tuple<
        >;

        using resp_type = std::string;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_get_current_line");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_set_current_line")>
    {
        using parms_tuple = std::tuple<
            std::string 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_set_current_line");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_del_current_line")>
    {
        using parms_tuple = std::tuple<
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_del_current_line");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_get_var")>
    {
        using parms_tuple = std::tuple<
            std::string 
        >;

        using resp_type = libnvc::object;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_get_var");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_set_var")>
    {
        using parms_tuple = std::tuple<
            std::string, 
            libnvc::object 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_set_var");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_del_var")>
    {
        using parms_tuple = std::tuple<
            std::string 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_del_var");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_get_vvar")>
    {
        using parms_tuple = std::tuple<
            std::string 
        >;

        using resp_type = libnvc::object;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_get_vvar");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_set_vvar")>
    {
        using parms_tuple = std::tuple<
            std::string, 
            libnvc::object 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 6;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_set_vvar");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_get_option")>
    {
        using parms_tuple = std::tuple<
            std::string 
        >;

        using resp_type = libnvc::object;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_get_option");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_set_option")>
    {
        using parms_tuple = std::tuple<
            std::string, 
            libnvc::object 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_set_option");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_out_write")>
    {
        using parms_tuple = std::tuple<
            std::string 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_out_write");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_err_write")>
    {
        using parms_tuple = std::tuple<
            std::string 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_err_write");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_err_writeln")>
    {
        using parms_tuple = std::tuple<
            std::string 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_err_writeln");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_list_bufs")>
    {
        using parms_tuple = std::tuple<
        >;

        using resp_type = std::vector<int64_t>;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_list_bufs");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_get_current_buf")>
    {
        using parms_tuple = std::tuple<
        >;

        using resp_type = int64_t;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_get_current_buf");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_set_current_buf")>
    {
        using parms_tuple = std::tuple<
            int64_t 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_set_current_buf");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_list_wins")>
    {
        using parms_tuple = std::tuple<
        >;

        using resp_type = std::vector<int64_t>;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_list_wins");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_get_current_win")>
    {
        using parms_tuple = std::tuple<
        >;

        using resp_type = int64_t;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_get_current_win");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_set_current_win")>
    {
        using parms_tuple = std::tuple<
            int64_t 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_set_current_win");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_create_buf")>
    {
        using parms_tuple = std::tuple<
            bool, 
            bool 
        >;

        using resp_type = int64_t;

        constexpr int since() const
        {
            return 6;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_create_buf");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_open_win")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            bool, 
            std::map<std::string, libnvc::object> 
        >;

        using resp_type = int64_t;

        constexpr int since() const
        {
            return 6;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_open_win");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_list_tabpages")>
    {
        using parms_tuple = std::tuple<
        >;

        using resp_type = std::vector<int64_t>;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_list_tabpages");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_get_current_tabpage")>
    {
        using parms_tuple = std::tuple<
        >;

        using resp_type = int64_t;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_get_current_tabpage");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_set_current_tabpage")>
    {
        using parms_tuple = std::tuple<
            int64_t 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_set_current_tabpage");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_create_namespace")>
    {
        using parms_tuple = std::tuple<
            std::string 
        >;

        using resp_type = int64_t;

        constexpr int since() const
        {
            return 5;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_create_namespace");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_get_namespaces")>
    {
        using parms_tuple = std::tuple<
        >;

        using resp_type = std::map<std::string, libnvc::object>;

        constexpr int since() const
        {
            return 5;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_get_namespaces");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_subscribe")>
    {
        using parms_tuple = std::tuple<
            std::string 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_subscribe");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_unsubscribe")>
    {
        using parms_tuple = std::tuple<
            std::string 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_unsubscribe");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_get_color_by_name")>
    {
        using parms_tuple = std::tuple<
            std::string 
        >;

        using resp_type = int64_t;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_get_color_by_name");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_get_color_map")>
    {
        using parms_tuple = std::tuple<
        >;

        using resp_type = std::map<std::string, libnvc::object>;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_get_color_map");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_get_mode")>
    {
        using parms_tuple = std::tuple<
        >;

        using resp_type = std::map<std::string, libnvc::object>;

        constexpr int since() const
        {
            return 2;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_get_mode");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_get_keymap")>
    {
        using parms_tuple = std::tuple<
            std::string 
        >;

        using resp_type = std::vector<std::map<std::string, libnvc::object>>;

        constexpr int since() const
        {
            return 3;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_get_keymap");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_get_commands")>
    {
        using parms_tuple = std::tuple<
            std::map<std::string, libnvc::object> 
        >;

        using resp_type = std::map<std::string, libnvc::object>;

        constexpr int since() const
        {
            return 4;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_get_commands");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_get_api_info")>
    {
        using parms_tuple = std::tuple<
        >;

        using resp_type = std::vector<libnvc::object>;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_get_api_info");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_set_client_info")>
    {
        using parms_tuple = std::tuple<
            std::string, 
            std::map<std::string, libnvc::object>, 
            std::string, 
            std::map<std::string, libnvc::object>, 
            std::map<std::string, libnvc::object> 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 4;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_set_client_info");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_get_chan_info")>
    {
        using parms_tuple = std::tuple<
            int64_t 
        >;

        using resp_type = std::map<std::string, libnvc::object>;

        constexpr int since() const
        {
            return 4;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_get_chan_info");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_list_chans")>
    {
        using parms_tuple = std::tuple<
        >;

        using resp_type = std::vector<libnvc::object>;

        constexpr int since() const
        {
            return 4;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_list_chans");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_call_atomic")>
    {
        using parms_tuple = std::tuple<
            std::vector<libnvc::object> 
        >;

        using resp_type = std::vector<libnvc::object>;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_call_atomic");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_parse_expression")>
    {
        using parms_tuple = std::tuple<
            std::string, 
            std::string, 
            bool 
        >;

        using resp_type = std::map<std::string, libnvc::object>;

        constexpr int since() const
        {
            return 4;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_parse_expression");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_list_uis")>
    {
        using parms_tuple = std::tuple<
        >;

        using resp_type = std::vector<libnvc::object>;

        constexpr int since() const
        {
            return 4;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_list_uis");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_get_proc_children")>
    {
        using parms_tuple = std::tuple<
            int64_t 
        >;

        using resp_type = std::vector<libnvc::object>;

        constexpr int since() const
        {
            return 4;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_get_proc_children");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_get_proc")>
    {
        using parms_tuple = std::tuple<
            int64_t 
        >;

        using resp_type = libnvc::object;

        constexpr int since() const
        {
            return 4;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_get_proc");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_select_popupmenu_item")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            bool, 
            bool, 
            std::map<std::string, libnvc::object> 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 6;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_select_popupmenu_item");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_win_get_buf")>
    {
        using parms_tuple = std::tuple<
            int64_t 
        >;

        using resp_type = int64_t;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_win_get_buf");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_win_set_buf")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            int64_t 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 5;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_win_set_buf");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_win_get_cursor")>
    {
        using parms_tuple = std::tuple<
            int64_t 
        >;

        using resp_type = std::array<int64_t, 2>;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_win_get_cursor");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_win_set_cursor")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            std::array<int64_t, 2> 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_win_set_cursor");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_win_get_height")>
    {
        using parms_tuple = std::tuple<
            int64_t 
        >;

        using resp_type = int64_t;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_win_get_height");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_win_set_height")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            int64_t 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_win_set_height");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_win_get_width")>
    {
        using parms_tuple = std::tuple<
            int64_t 
        >;

        using resp_type = int64_t;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_win_get_width");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_win_set_width")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            int64_t 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_win_set_width");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_win_get_var")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            std::string 
        >;

        using resp_type = libnvc::object;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_win_get_var");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_win_set_var")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            std::string, 
            libnvc::object 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_win_set_var");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_win_del_var")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            std::string 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_win_del_var");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_win_get_option")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            std::string 
        >;

        using resp_type = libnvc::object;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_win_get_option");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_win_set_option")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            std::string, 
            libnvc::object 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_win_set_option");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_win_get_position")>
    {
        using parms_tuple = std::tuple<
            int64_t 
        >;

        using resp_type = std::array<int64_t, 2>;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_win_get_position");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_win_get_tabpage")>
    {
        using parms_tuple = std::tuple<
            int64_t 
        >;

        using resp_type = int64_t;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_win_get_tabpage");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_win_get_number")>
    {
        using parms_tuple = std::tuple<
            int64_t 
        >;

        using resp_type = int64_t;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_win_get_number");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_win_is_valid")>
    {
        using parms_tuple = std::tuple<
            int64_t 
        >;

        using resp_type = bool;

        constexpr int since() const
        {
            return 1;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_win_is_valid");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_win_set_config")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            std::map<std::string, libnvc::object> 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 6;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_win_set_config");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_win_get_config")>
    {
        using parms_tuple = std::tuple<
            int64_t 
        >;

        using resp_type = std::map<std::string, libnvc::object>;

        constexpr int since() const
        {
            return 6;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_win_get_config");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
    template<> struct req<libnvc::reqid("nvim_win_close")>
    {
        using parms_tuple = std::tuple<
            int64_t, 
            bool 
        >;

        using resp_type = void;

        constexpr int since() const
        {
            return 6;
        }

        constexpr size_t id() const
        {
            return libnvc::strid("req::nvim_win_close");
        }

        constexpr const char *name() const
        {
            return reqstr(id());
        }
    };
}

namespace libnvc
{
    class api_client
    {
        private:
            class stream_decoder;

        private:
            std::unique_ptr<stream_decoder> m_decoder;

        private:
            int64_t m_seqid;

        private:
            libnvc::io_device *m_iodev;

        private:
            std::map<int64_t, std::function<void(libnvc::resp_variant)>> m_onresp;
            std::map<int64_t, std::function<void(int64_t, std::string)>> m_onresperr;

        public:
            api_client(libnvc::io_device *);

        public:
            // seems I can't use the default dtor
            // because of the incomplete type stream_decoder and unique_ptr
            ~api_client();

        private:
            const char *build_signature() const
            {
                return "59d131f2-129e-4c24-a0d3-34b3d9b301b8";
            }

        public:
            int64_t seqid() const
            {
                return m_seqid;
            }

        private:
            void add_seqid(int64_t add)
            {
                m_seqid += add;
            }

        private:

        private:
            template<size_t reqid, typename on_resp_t> inline void regcb_resp(on_resp_t on_resp)
            {
                auto msg_id = libnvc::msgid(reqid, seqid());
                if(true /* on_resp */){
                    if(m_onresp.find(msg_id) != m_onresp.end()){
                        throw std::runtime_error(((std::string("response handler already resgistered: req = ") + libnvc::idstr(reqid)) + ", seqid = ") + std::to_string(seqid()));
                    }

                    m_onresp[msg_id] = [this, on_resp](libnvc::resp_variant result)
                    {
                        if constexpr (std::is_void_v<typename libnvc::req<reqid>::resp_type>){
                            on_resp();
                        }else{
                            on_resp(std::get<typename libnvc::req<reqid>::resp_type>(result));
                        }
                    };
                }
            }

            template<size_t reqid, typename on_resperr_t> inline void regcb_resperr(on_resperr_t on_resperr)
            {
                auto msg_id = libnvc::msgid(reqid, seqid());
                if(true /* on_resperr */){
                    if(m_onresperr.find(msg_id) != m_onresperr.end()){
                        throw std::runtime_error(((std::string("response error handler already resgistered: req = ") + libnvc::idstr(reqid)) + ", seqid = ") + std::to_string(seqid()));
                    }
                    m_onresperr[msg_id] = on_resperr;
                }
            }

        public:
            template<size_t reqid, typename on_resp_t> inline int64_t forward(const typename libnvc::req<reqid>::parms_tuple &parms, on_resp_t on_resp)
            {
                add_seqid(1);
                auto msgid = libnvc::msgid(reqid, seqid());

                libnvc::mpinterf::writer packer;
                auto fn_pack = [&packer](int64_t msgid, auto &&... args)
                {
                    return packer.pack_req(msgid, std::forward<decltype(args)>(args)...);
                };

                m_iodev->send(std::apply(fn_pack, std::tuple_cat(std::make_tuple(msgid), parms)));
                regcb_resp<reqid, on_resp_t>(on_resp);
                return msgid;
            }

            template<size_t reqid, typename on_resp_t, typename on_resperr_t> inline int64_t forward(const typename libnvc::req<reqid>::parms_tuple &parms, on_resp_t on_resp, on_resperr_t on_resperr)
            {
                add_seqid(1);
                auto msgid = libnvc::msgid(reqid, seqid());

                libnvc::mpinterf::writer packer;
                auto fn_pack = [&packer](int64_t msgid, auto &&... args)
                {
                    return packer.pack_req(msgid, std::forward<decltype(args)>(args)...);
                };

                m_iodev->send(std::apply(fn_pack, std::tuple_cat(std::make_tuple(msgid), parms)));
                regcb_resp<reqid, on_resp_t>(on_resp);
                regcb_resperr<reqid, on_resperr_t>(on_resperr);
                return msgid;
            }

        public:
            void poll()
            {
                while(poll_one()) continue;
            }

        public:
            int64_t poll_one();

        public:
            int64_t poll_any()
            {
                while(true){
                    if(auto msgid = poll_one(); msgid != 0){
                        return msgid;
                    }
                }
                return 0;
            }

        public:
            void poll_all()
            {
                while(!m_onresp.empty() || !m_onresperr.empty()){
                    poll();
                }
            }

        public:
            void poll_wait(int64_t msgid)
            {
                while(true){
                    if(msgid == poll_one()){
                        break;
                    }
                }
            }

        public:
            // blocking API
            // this may be necessary for requests like nvim_ui_attach()
            // when got error reported from nvim server these function throw [errc, errmsg]

            int64_t nvim_buf_line_count(int64_t buffer)
            {
                int64_t r;
                auto on_resp = [&r](int64_t res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_buf_line_count\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_buf_line_count");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(buffer), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            bool nvim_buf_attach(int64_t buffer, bool send_buffer, const std::map<std::string, libnvc::object> & opts)
            {
                bool r;
                auto on_resp = [&r](bool res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_buf_attach\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_buf_attach");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(buffer, send_buffer, opts), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            bool nvim_buf_detach(int64_t buffer)
            {
                bool r;
                auto on_resp = [&r](bool res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_buf_detach\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_buf_detach");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(buffer), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            std::vector<std::string> nvim_buf_get_lines(int64_t buffer, int64_t start, int64_t end, bool strict_indexing)
            {
                std::vector<std::string> r;
                auto on_resp = [&r](std::vector<std::string> res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_buf_get_lines\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_buf_get_lines");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(buffer, start, end, strict_indexing), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_buf_set_lines(int64_t buffer, int64_t start, int64_t end, bool strict_indexing, const std::vector<std::string> & replacement)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_buf_set_lines\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_buf_set_lines");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(buffer, start, end, strict_indexing, replacement), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            int64_t nvim_buf_get_offset(int64_t buffer, int64_t index)
            {
                int64_t r;
                auto on_resp = [&r](int64_t res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_buf_get_offset\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_buf_get_offset");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(buffer, index), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            libnvc::object nvim_buf_get_var(int64_t buffer, const std::string & name)
            {
                libnvc::object r;
                auto on_resp = [&r](libnvc::object res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_buf_get_var\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_buf_get_var");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(buffer, name), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            int64_t nvim_buf_get_changedtick(int64_t buffer)
            {
                int64_t r;
                auto on_resp = [&r](int64_t res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_buf_get_changedtick\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_buf_get_changedtick");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(buffer), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            std::vector<std::map<std::string, libnvc::object>> nvim_buf_get_keymap(int64_t buffer, const std::string & mode)
            {
                std::vector<std::map<std::string, libnvc::object>> r;
                auto on_resp = [&r](std::vector<std::map<std::string, libnvc::object>> res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_buf_get_keymap\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_buf_get_keymap");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(buffer, mode), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            std::map<std::string, libnvc::object> nvim_buf_get_commands(int64_t buffer, const std::map<std::string, libnvc::object> & opts)
            {
                std::map<std::string, libnvc::object> r;
                auto on_resp = [&r](std::map<std::string, libnvc::object> res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_buf_get_commands\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_buf_get_commands");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(buffer, opts), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_buf_set_var(int64_t buffer, const std::string & name, libnvc::object value)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_buf_set_var\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_buf_set_var");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(buffer, name, value), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            void nvim_buf_del_var(int64_t buffer, const std::string & name)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_buf_del_var\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_buf_del_var");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(buffer, name), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            libnvc::object nvim_buf_get_option(int64_t buffer, const std::string & name)
            {
                libnvc::object r;
                auto on_resp = [&r](libnvc::object res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_buf_get_option\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_buf_get_option");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(buffer, name), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_buf_set_option(int64_t buffer, const std::string & name, libnvc::object value)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_buf_set_option\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_buf_set_option");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(buffer, name, value), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            std::string nvim_buf_get_name(int64_t buffer)
            {
                std::string r;
                auto on_resp = [&r](std::string res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_buf_get_name\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_buf_get_name");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(buffer), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_buf_set_name(int64_t buffer, const std::string & name)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_buf_set_name\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_buf_set_name");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(buffer, name), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            bool nvim_buf_is_loaded(int64_t buffer)
            {
                bool r;
                auto on_resp = [&r](bool res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_buf_is_loaded\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_buf_is_loaded");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(buffer), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            bool nvim_buf_is_valid(int64_t buffer)
            {
                bool r;
                auto on_resp = [&r](bool res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_buf_is_valid\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_buf_is_valid");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(buffer), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            std::array<int64_t, 2> nvim_buf_get_mark(int64_t buffer, const std::string & name)
            {
                std::array<int64_t, 2> r;
                auto on_resp = [&r](std::array<int64_t, 2> res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_buf_get_mark\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_buf_get_mark");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(buffer, name), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            int64_t nvim_buf_add_highlight(int64_t buffer, int64_t ns_id, const std::string & hl_group, int64_t line, int64_t col_start, int64_t col_end)
            {
                int64_t r;
                auto on_resp = [&r](int64_t res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_buf_add_highlight\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_buf_add_highlight");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(buffer, ns_id, hl_group, line, col_start, col_end), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_buf_clear_namespace(int64_t buffer, int64_t ns_id, int64_t line_start, int64_t line_end)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_buf_clear_namespace\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_buf_clear_namespace");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(buffer, ns_id, line_start, line_end), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            void nvim_buf_clear_highlight(int64_t buffer, int64_t ns_id, int64_t line_start, int64_t line_end)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_buf_clear_highlight\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_buf_clear_highlight");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(buffer, ns_id, line_start, line_end), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            int64_t nvim_buf_set_virtual_text(int64_t buffer, int64_t ns_id, int64_t line, const std::vector<libnvc::object> & chunks, const std::map<std::string, libnvc::object> & opts)
            {
                int64_t r;
                auto on_resp = [&r](int64_t res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_buf_set_virtual_text\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_buf_set_virtual_text");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(buffer, ns_id, line, chunks, opts), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            std::vector<int64_t> nvim_tabpage_list_wins(int64_t tabpage)
            {
                std::vector<int64_t> r;
                auto on_resp = [&r](std::vector<int64_t> res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_tabpage_list_wins\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_tabpage_list_wins");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(tabpage), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            libnvc::object nvim_tabpage_get_var(int64_t tabpage, const std::string & name)
            {
                libnvc::object r;
                auto on_resp = [&r](libnvc::object res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_tabpage_get_var\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_tabpage_get_var");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(tabpage, name), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_tabpage_set_var(int64_t tabpage, const std::string & name, libnvc::object value)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_tabpage_set_var\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_tabpage_set_var");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(tabpage, name, value), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            void nvim_tabpage_del_var(int64_t tabpage, const std::string & name)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_tabpage_del_var\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_tabpage_del_var");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(tabpage, name), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            int64_t nvim_tabpage_get_win(int64_t tabpage)
            {
                int64_t r;
                auto on_resp = [&r](int64_t res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_tabpage_get_win\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_tabpage_get_win");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(tabpage), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            int64_t nvim_tabpage_get_number(int64_t tabpage)
            {
                int64_t r;
                auto on_resp = [&r](int64_t res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_tabpage_get_number\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_tabpage_get_number");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(tabpage), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            bool nvim_tabpage_is_valid(int64_t tabpage)
            {
                bool r;
                auto on_resp = [&r](bool res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_tabpage_is_valid\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_tabpage_is_valid");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(tabpage), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_ui_attach(int64_t width, int64_t height, const std::map<std::string, libnvc::object> & options)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_ui_attach\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_ui_attach");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(width, height, options), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            void nvim_ui_detach()
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_ui_detach\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_ui_detach");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            void nvim_ui_try_resize(int64_t width, int64_t height)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_ui_try_resize\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_ui_try_resize");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(width, height), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            void nvim_ui_set_option(const std::string & name, libnvc::object value)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_ui_set_option\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_ui_set_option");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(name, value), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            void nvim_ui_try_resize_grid(int64_t grid, int64_t width, int64_t height)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_ui_try_resize_grid\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_ui_try_resize_grid");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(grid, width, height), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            void nvim_command(const std::string & command)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_command\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_command");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(command), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            std::map<std::string, libnvc::object> nvim_get_hl_by_name(const std::string & name, bool rgb)
            {
                std::map<std::string, libnvc::object> r;
                auto on_resp = [&r](std::map<std::string, libnvc::object> res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_get_hl_by_name\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_get_hl_by_name");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(name, rgb), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            std::map<std::string, libnvc::object> nvim_get_hl_by_id(int64_t hl_id, bool rgb)
            {
                std::map<std::string, libnvc::object> r;
                auto on_resp = [&r](std::map<std::string, libnvc::object> res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_get_hl_by_id\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_get_hl_by_id");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(hl_id, rgb), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_feedkeys(const std::string & keys, const std::string & mode, bool escape_csi)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_feedkeys\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_feedkeys");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(keys, mode, escape_csi), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            int64_t nvim_input(const std::string & keys)
            {
                int64_t r;
                auto on_resp = [&r](int64_t res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_input\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_input");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(keys), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_input_mouse(const std::string & button, const std::string & action, const std::string & modifier, int64_t grid, int64_t row, int64_t col)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_input_mouse\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_input_mouse");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(button, action, modifier, grid, row, col), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            std::string nvim_replace_termcodes(const std::string & str, bool from_part, bool do_lt, bool special)
            {
                std::string r;
                auto on_resp = [&r](std::string res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_replace_termcodes\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_replace_termcodes");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(str, from_part, do_lt, special), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            std::string nvim_command_output(const std::string & command)
            {
                std::string r;
                auto on_resp = [&r](std::string res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_command_output\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_command_output");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(command), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            libnvc::object nvim_eval(const std::string & expr)
            {
                libnvc::object r;
                auto on_resp = [&r](libnvc::object res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_eval\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_eval");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(expr), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            libnvc::object nvim_execute_lua(const std::string & code, const std::vector<libnvc::object> & args)
            {
                libnvc::object r;
                auto on_resp = [&r](libnvc::object res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_execute_lua\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_execute_lua");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(code, args), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            libnvc::object nvim_call_function(const std::string & fn, const std::vector<libnvc::object> & args)
            {
                libnvc::object r;
                auto on_resp = [&r](libnvc::object res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_call_function\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_call_function");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(fn, args), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            libnvc::object nvim_call_dict_function(libnvc::object dict, const std::string & fn, const std::vector<libnvc::object> & args)
            {
                libnvc::object r;
                auto on_resp = [&r](libnvc::object res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_call_dict_function\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_call_dict_function");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(dict, fn, args), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            int64_t nvim_strwidth(const std::string & text)
            {
                int64_t r;
                auto on_resp = [&r](int64_t res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_strwidth\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_strwidth");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(text), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            std::vector<std::string> nvim_list_runtime_paths()
            {
                std::vector<std::string> r;
                auto on_resp = [&r](std::vector<std::string> res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_list_runtime_paths\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_list_runtime_paths");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_set_current_dir(const std::string & dir)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_set_current_dir\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_set_current_dir");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(dir), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            std::string nvim_get_current_line()
            {
                std::string r;
                auto on_resp = [&r](std::string res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_get_current_line\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_get_current_line");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_set_current_line(const std::string & line)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_set_current_line\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_set_current_line");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(line), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            void nvim_del_current_line()
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_del_current_line\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_del_current_line");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            libnvc::object nvim_get_var(const std::string & name)
            {
                libnvc::object r;
                auto on_resp = [&r](libnvc::object res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_get_var\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_get_var");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(name), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_set_var(const std::string & name, libnvc::object value)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_set_var\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_set_var");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(name, value), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            void nvim_del_var(const std::string & name)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_del_var\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_del_var");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(name), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            libnvc::object nvim_get_vvar(const std::string & name)
            {
                libnvc::object r;
                auto on_resp = [&r](libnvc::object res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_get_vvar\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_get_vvar");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(name), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_set_vvar(const std::string & name, libnvc::object value)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_set_vvar\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_set_vvar");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(name, value), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            libnvc::object nvim_get_option(const std::string & name)
            {
                libnvc::object r;
                auto on_resp = [&r](libnvc::object res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_get_option\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_get_option");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(name), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_set_option(const std::string & name, libnvc::object value)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_set_option\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_set_option");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(name, value), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            void nvim_out_write(const std::string & str)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_out_write\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_out_write");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(str), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            void nvim_err_write(const std::string & str)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_err_write\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_err_write");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(str), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            void nvim_err_writeln(const std::string & str)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_err_writeln\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_err_writeln");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(str), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            std::vector<int64_t> nvim_list_bufs()
            {
                std::vector<int64_t> r;
                auto on_resp = [&r](std::vector<int64_t> res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_list_bufs\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_list_bufs");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            int64_t nvim_get_current_buf()
            {
                int64_t r;
                auto on_resp = [&r](int64_t res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_get_current_buf\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_get_current_buf");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_set_current_buf(int64_t buffer)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_set_current_buf\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_set_current_buf");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(buffer), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            std::vector<int64_t> nvim_list_wins()
            {
                std::vector<int64_t> r;
                auto on_resp = [&r](std::vector<int64_t> res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_list_wins\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_list_wins");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            int64_t nvim_get_current_win()
            {
                int64_t r;
                auto on_resp = [&r](int64_t res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_get_current_win\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_get_current_win");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_set_current_win(int64_t window)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_set_current_win\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_set_current_win");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(window), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            int64_t nvim_create_buf(bool listed, bool scratch)
            {
                int64_t r;
                auto on_resp = [&r](int64_t res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_create_buf\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_create_buf");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(listed, scratch), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            int64_t nvim_open_win(int64_t buffer, bool enter, const std::map<std::string, libnvc::object> & config)
            {
                int64_t r;
                auto on_resp = [&r](int64_t res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_open_win\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_open_win");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(buffer, enter, config), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            std::vector<int64_t> nvim_list_tabpages()
            {
                std::vector<int64_t> r;
                auto on_resp = [&r](std::vector<int64_t> res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_list_tabpages\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_list_tabpages");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            int64_t nvim_get_current_tabpage()
            {
                int64_t r;
                auto on_resp = [&r](int64_t res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_get_current_tabpage\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_get_current_tabpage");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_set_current_tabpage(int64_t tabpage)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_set_current_tabpage\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_set_current_tabpage");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(tabpage), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            int64_t nvim_create_namespace(const std::string & name)
            {
                int64_t r;
                auto on_resp = [&r](int64_t res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_create_namespace\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_create_namespace");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(name), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            std::map<std::string, libnvc::object> nvim_get_namespaces()
            {
                std::map<std::string, libnvc::object> r;
                auto on_resp = [&r](std::map<std::string, libnvc::object> res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_get_namespaces\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_get_namespaces");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_subscribe(const std::string & event)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_subscribe\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_subscribe");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(event), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            void nvim_unsubscribe(const std::string & event)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_unsubscribe\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_unsubscribe");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(event), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            int64_t nvim_get_color_by_name(const std::string & name)
            {
                int64_t r;
                auto on_resp = [&r](int64_t res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_get_color_by_name\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_get_color_by_name");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(name), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            std::map<std::string, libnvc::object> nvim_get_color_map()
            {
                std::map<std::string, libnvc::object> r;
                auto on_resp = [&r](std::map<std::string, libnvc::object> res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_get_color_map\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_get_color_map");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            std::map<std::string, libnvc::object> nvim_get_mode()
            {
                std::map<std::string, libnvc::object> r;
                auto on_resp = [&r](std::map<std::string, libnvc::object> res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_get_mode\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_get_mode");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            std::vector<std::map<std::string, libnvc::object>> nvim_get_keymap(const std::string & mode)
            {
                std::vector<std::map<std::string, libnvc::object>> r;
                auto on_resp = [&r](std::vector<std::map<std::string, libnvc::object>> res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_get_keymap\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_get_keymap");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(mode), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            std::map<std::string, libnvc::object> nvim_get_commands(const std::map<std::string, libnvc::object> & opts)
            {
                std::map<std::string, libnvc::object> r;
                auto on_resp = [&r](std::map<std::string, libnvc::object> res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_get_commands\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_get_commands");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(opts), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            std::vector<libnvc::object> nvim_get_api_info()
            {
                std::vector<libnvc::object> r;
                auto on_resp = [&r](std::vector<libnvc::object> res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_get_api_info\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_get_api_info");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_set_client_info(const std::string & name, const std::map<std::string, libnvc::object> & version, const std::string & type, const std::map<std::string, libnvc::object> & methods, const std::map<std::string, libnvc::object> & attributes)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_set_client_info\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_set_client_info");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(name, version, type, methods, attributes), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            std::map<std::string, libnvc::object> nvim_get_chan_info(int64_t chan)
            {
                std::map<std::string, libnvc::object> r;
                auto on_resp = [&r](std::map<std::string, libnvc::object> res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_get_chan_info\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_get_chan_info");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(chan), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            std::vector<libnvc::object> nvim_list_chans()
            {
                std::vector<libnvc::object> r;
                auto on_resp = [&r](std::vector<libnvc::object> res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_list_chans\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_list_chans");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            std::vector<libnvc::object> nvim_call_atomic(const std::vector<libnvc::object> & calls)
            {
                std::vector<libnvc::object> r;
                auto on_resp = [&r](std::vector<libnvc::object> res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_call_atomic\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_call_atomic");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(calls), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            std::map<std::string, libnvc::object> nvim_parse_expression(const std::string & expr, const std::string & flags, bool highlight)
            {
                std::map<std::string, libnvc::object> r;
                auto on_resp = [&r](std::map<std::string, libnvc::object> res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_parse_expression\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_parse_expression");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(expr, flags, highlight), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            std::vector<libnvc::object> nvim_list_uis()
            {
                std::vector<libnvc::object> r;
                auto on_resp = [&r](std::vector<libnvc::object> res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_list_uis\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_list_uis");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            std::vector<libnvc::object> nvim_get_proc_children(int64_t pid)
            {
                std::vector<libnvc::object> r;
                auto on_resp = [&r](std::vector<libnvc::object> res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_get_proc_children\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_get_proc_children");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(pid), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            libnvc::object nvim_get_proc(int64_t pid)
            {
                libnvc::object r;
                auto on_resp = [&r](libnvc::object res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_get_proc\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_get_proc");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(pid), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_select_popupmenu_item(int64_t item, bool insert, bool finish, const std::map<std::string, libnvc::object> & opts)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_select_popupmenu_item\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_select_popupmenu_item");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(item, insert, finish, opts), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            int64_t nvim_win_get_buf(int64_t window)
            {
                int64_t r;
                auto on_resp = [&r](int64_t res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_win_get_buf\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_win_get_buf");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(window), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_win_set_buf(int64_t window, int64_t buffer)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_win_set_buf\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_win_set_buf");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(window, buffer), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            std::array<int64_t, 2> nvim_win_get_cursor(int64_t window)
            {
                std::array<int64_t, 2> r;
                auto on_resp = [&r](std::array<int64_t, 2> res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_win_get_cursor\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_win_get_cursor");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(window), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_win_set_cursor(int64_t window, std::array<int64_t, 2> pos)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_win_set_cursor\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_win_set_cursor");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(window, pos), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            int64_t nvim_win_get_height(int64_t window)
            {
                int64_t r;
                auto on_resp = [&r](int64_t res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_win_get_height\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_win_get_height");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(window), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_win_set_height(int64_t window, int64_t height)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_win_set_height\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_win_set_height");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(window, height), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            int64_t nvim_win_get_width(int64_t window)
            {
                int64_t r;
                auto on_resp = [&r](int64_t res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_win_get_width\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_win_get_width");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(window), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_win_set_width(int64_t window, int64_t width)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_win_set_width\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_win_set_width");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(window, width), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            libnvc::object nvim_win_get_var(int64_t window, const std::string & name)
            {
                libnvc::object r;
                auto on_resp = [&r](libnvc::object res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_win_get_var\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_win_get_var");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(window, name), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_win_set_var(int64_t window, const std::string & name, libnvc::object value)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_win_set_var\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_win_set_var");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(window, name, value), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            void nvim_win_del_var(int64_t window, const std::string & name)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_win_del_var\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_win_del_var");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(window, name), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            libnvc::object nvim_win_get_option(int64_t window, const std::string & name)
            {
                libnvc::object r;
                auto on_resp = [&r](libnvc::object res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_win_get_option\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_win_get_option");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(window, name), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_win_set_option(int64_t window, const std::string & name, libnvc::object value)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_win_set_option\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_win_set_option");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(window, name, value), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            std::array<int64_t, 2> nvim_win_get_position(int64_t window)
            {
                std::array<int64_t, 2> r;
                auto on_resp = [&r](std::array<int64_t, 2> res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_win_get_position\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_win_get_position");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(window), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            int64_t nvim_win_get_tabpage(int64_t window)
            {
                int64_t r;
                auto on_resp = [&r](int64_t res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_win_get_tabpage\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_win_get_tabpage");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(window), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            int64_t nvim_win_get_number(int64_t window)
            {
                int64_t r;
                auto on_resp = [&r](int64_t res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_win_get_number\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_win_get_number");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(window), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            bool nvim_win_is_valid(int64_t window)
            {
                bool r;
                auto on_resp = [&r](bool res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_win_is_valid\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_win_is_valid");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(window), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_win_set_config(int64_t window, const std::map<std::string, libnvc::object> & config)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_win_set_config\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_win_set_config");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(window, config), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }

            std::map<std::string, libnvc::object> nvim_win_get_config(int64_t window)
            {
                std::map<std::string, libnvc::object> r;
                auto on_resp = [&r](std::map<std::string, libnvc::object> res)
                {
                    r = res;
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_win_get_config\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_win_get_config");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(window), on_resp, on_resperr);
                poll_wait(msgid);
                return r;
            }

            void nvim_win_close(int64_t window, bool force)
            {
                auto on_resp = []()
                {
                };
                auto on_resperr = [](int64_t ec, std::string emsg)
                {
                    throw std::runtime_error(((std::string("Error for request \"nvim_win_close\": [") + std::to_string(ec) + ", ") + emsg) + "]");
                };

                constexpr size_t req_id = libnvc::reqid("nvim_win_close");
                auto msgid = forward<req_id>(libnvc::req<req_id>::parms_tuple(window, force), on_resp, on_resperr);
                poll_wait(msgid);
                return;
            }


        public:
            // ui notifications
            // notif doesn't have return value, do nothing by default
            // it's impossible for jinja2 to generate proper code for each notification
            virtual void on_mode_info_set(bool, const std::vector<libnvc::object> &)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: mode_info_set");
            }
            virtual void on_update_menu()
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: update_menu");
            }
            virtual void on_busy_start()
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: busy_start");
            }
            virtual void on_busy_stop()
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: busy_stop");
            }
            virtual void on_mouse_on()
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: mouse_on");
            }
            virtual void on_mouse_off()
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: mouse_off");
            }
            virtual void on_mode_change(const std::string &, int64_t)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: mode_change");
            }
            virtual void on_bell()
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: bell");
            }
            virtual void on_visual_bell()
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: visual_bell");
            }
            virtual void on_flush()
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: flush");
            }
            virtual void on_suspend()
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: suspend");
            }
            virtual void on_set_title(const std::string &)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: set_title");
            }
            virtual void on_set_icon(const std::string &)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: set_icon");
            }
            virtual void on_option_set(const std::string &, libnvc::object)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: option_set");
            }
            virtual void on_update_fg(int64_t)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: update_fg");
            }
            virtual void on_update_bg(int64_t)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: update_bg");
            }
            virtual void on_update_sp(int64_t)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: update_sp");
            }
            virtual void on_resize(int64_t, int64_t)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: resize");
            }
            virtual void on_clear()
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: clear");
            }
            virtual void on_eol_clear()
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: eol_clear");
            }
            virtual void on_cursor_goto(int64_t, int64_t)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: cursor_goto");
            }
            virtual void on_highlight_set(const std::map<std::string, libnvc::object> &)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: highlight_set");
            }
            virtual void on_put(const std::string &)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: put");
            }
            virtual void on_set_scroll_region(int64_t, int64_t, int64_t, int64_t)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: set_scroll_region");
            }
            virtual void on_scroll(int64_t)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: scroll");
            }
            virtual void on_default_colors_set(int64_t, int64_t, int64_t, int64_t, int64_t)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: default_colors_set");
            }
            virtual void on_hl_attr_define(int64_t, const std::map<std::string, libnvc::object> &, const std::map<std::string, libnvc::object> &, const std::vector<libnvc::object> &)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: hl_attr_define");
            }
            virtual void on_grid_resize(int64_t, int64_t, int64_t)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: grid_resize");
            }
            virtual void on_grid_clear(int64_t)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: grid_clear");
            }
            virtual void on_grid_cursor_goto(int64_t, int64_t, int64_t)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: grid_cursor_goto");
            }
            virtual void on_grid_line(int64_t, int64_t, int64_t, const std::vector<libnvc::object> &)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: grid_line");
            }
            virtual void on_grid_scroll(int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: grid_scroll");
            }
            virtual void on_grid_destroy(int64_t)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: grid_destroy");
            }
            virtual void on_win_pos(int64_t, int64_t, int64_t, int64_t, int64_t, int64_t)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: win_pos");
            }
            virtual void on_win_float_pos(int64_t, int64_t, const std::string &, int64_t, double, double, bool)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: win_float_pos");
            }
            virtual void on_win_external_pos(int64_t, int64_t)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: win_external_pos");
            }
            virtual void on_win_hide(int64_t)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: win_hide");
            }
            virtual void on_win_close(int64_t)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: win_close");
            }
            virtual void on_win_scroll_over_start()
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: win_scroll_over_start");
            }
            virtual void on_win_scroll_over_reset()
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: win_scroll_over_reset");
            }
            virtual void on_popupmenu_show(const std::vector<libnvc::object> &, int64_t, int64_t, int64_t, int64_t)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: popupmenu_show");
            }
            virtual void on_popupmenu_hide()
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: popupmenu_hide");
            }
            virtual void on_popupmenu_select(int64_t)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: popupmenu_select");
            }
            virtual void on_tabline_update(int64_t, const std::vector<libnvc::object> &)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: tabline_update");
            }
            virtual void on_cmdline_show(const std::vector<libnvc::object> &, int64_t, const std::string &, const std::string &, int64_t, int64_t)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: cmdline_show");
            }
            virtual void on_cmdline_pos(int64_t, int64_t)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: cmdline_pos");
            }
            virtual void on_cmdline_special_char(const std::string &, bool, int64_t)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: cmdline_special_char");
            }
            virtual void on_cmdline_hide(int64_t)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: cmdline_hide");
            }
            virtual void on_cmdline_block_show(const std::vector<libnvc::object> &)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: cmdline_block_show");
            }
            virtual void on_cmdline_block_append(const std::vector<libnvc::object> &)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: cmdline_block_append");
            }
            virtual void on_cmdline_block_hide()
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: cmdline_block_hide");
            }
            virtual void on_wildmenu_show(const std::vector<libnvc::object> &)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: wildmenu_show");
            }
            virtual void on_wildmenu_select(int64_t)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: wildmenu_select");
            }
            virtual void on_wildmenu_hide()
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: wildmenu_hide");
            }
            virtual void on_msg_show(const std::string &, const std::vector<libnvc::object> &, bool)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: msg_show");
            }
            virtual void on_msg_clear()
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: msg_clear");
            }
            virtual void on_msg_showcmd(const std::vector<libnvc::object> &)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: msg_showcmd");
            }
            virtual void on_msg_showmode(const std::vector<libnvc::object> &)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: msg_showmode");
            }
            virtual void on_msg_ruler(const std::vector<libnvc::object> &)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: msg_ruler");
            }
            virtual void on_msg_history_show(const std::vector<libnvc::object> &)
            {
                libnvc::log(libnvc::LOG_DEBUG, "Ignored notif: msg_history_show");
            }

    };
}

namespace libnvc
{
    std::string print_object(const libnvc::object &);
}

struct libnvc_rect
{
    int x;
    int y;
    int w;
    int h;
};

namespace libnvc
{
    // make it as a class
    // can't initialize by std::memset(0) since 0x00000000 is valid utf8 coding
    class CELL final
    {
        private:
            // is to define a invalid CELL use this
            // for any other we assume the input is a valid utf8code
            // 
            // user shoun't know this value
            // use clear() to clear one existing cell
            constexpr static uint32_t INVALID_UTF8 = 0XFFFFFFFF;

        private:
            uint32_t m_utf8code;

        private:
            uint32_t m_hlid    : 30;
            uint32_t m_wcwidth :  2;

        public:
            CELL(uint32_t, int);

        public:
            CELL(): CELL(INVALID_UTF8, 0) {}
            CELL(uint32_t utf8code): CELL(utf8code, 0) {}

        public:
            void clear()
            {
                m_utf8code = INVALID_UTF8;
            }

            void set(uint32_t, int);

        public:
            bool valid() const
            {
                return m_utf8code != INVALID_UTF8;
            }

        public:
            size_t wc_width() const
            {
                return m_wcwidth;
            }

        public:
            int hl_id() const
            {
                if(valid()){
                    return m_hlid;
                }
                return 0;
            }

        public:
            const char *len4_cstr() const
            {
                if(valid()){
                    return (const char *)(&m_utf8code);
                }
                return nullptr;
            }
    };

    class board
    {
        private:
            friend class nvim_client;

        private:
            size_t m_grid_width;
            size_t m_grid_height;

        private:
            size_t m_cursor_x;
            size_t m_cursor_y;

        protected:
            std::vector<libnvc::CELL> m_cells;

        public:
            board(size_t grid_width, size_t grid_height)
                : m_grid_width(grid_width)
                , m_grid_height(grid_height)
                , m_cursor_x(0)
                , m_cursor_y(0)
                , m_cells(grid_width * grid_height)
            {}

        public:
            void clear()
            {
                for(auto &cell: m_cells){
                    cell.clear();
                }
            }

            void clear_char()
            {
                for(auto &cell: m_cells){
                    cell.set(' ', 0);
                }
            }

        public:
            size_t cursor_x() const
            {
                return m_cursor_x;
            }

            size_t cursor_y() const
            {
                return m_cursor_y;
            }

            size_t width() const
            {
                return m_grid_width;
            }

            size_t height() const
            {
                return m_grid_height;
            }

        private:
            size_t cursor_off() const
            {
                return cursor_y() * width() + cursor_x();
            }

        public:
            void set_cursor(size_t loc)
            {
                m_cursor_x = loc % width();
                m_cursor_y = loc / width();
            }

            void set_cursor(size_t x, size_t y)
            {
                m_cursor_x = x;
                m_cursor_y = y;
            }

        public:
            void advance_cursor(size_t pos)
            {
                set_cursor(cursor_off() + pos);
            }

        public:
            std::unique_ptr<board> clone() const
            {
                return std::make_unique<board>(*this);
            }

        private:
            CELL &get_cell(size_t x, size_t y)
            {
                return m_cells.at(y * width() + x);
            }

        public:
            const CELL &get_cell(size_t x, size_t y) const
            {
                return m_cells.at(y * width() + x);
            }

        private:
            CELL &get_cell()
            {
                return m_cells.at(cursor_off());
            }

        public:
            const CELL &get_cell() const
            {
                return m_cells.at(cursor_off());
            }
    };

    class nvim_client: public api_client
    {
        private:
            friend class nvim_widget;

        public:
            struct hl_attrdef
            {
                uint32_t color_fg : 24;
                uint32_t color_bg : 24;
                uint32_t color_sp : 24;

                uint32_t color_fg_defined : 1;
                uint32_t color_bg_defined : 1;
                uint32_t color_sp_defined : 1;

                uint32_t reverse   : 1;
                uint32_t italic    : 1;
                uint32_t bold      : 1;
                uint32_t underline : 1;
                uint32_t undercurl : 1;

                hl_attrdef()
                {
                    clear();
                }

                void clear()
                {
                    std::memset(this, 0, sizeof(hl_attrdef));
                }
            };

        protected:
            struct mode_infodef
            {
                int attr_id         = 0;
                int cell_percentage = 0;

                std::string name         = "";
                std::string short_name   = "";
                std::string cursor_shape = "";
            };

        private:
            std::vector<hl_attrdef> m_hldefs;

        private:
            int  m_mode;
            bool m_cursor_style_enabled;

        private:
            std::vector<mode_infodef> m_modedefs;

        private:
            std::unordered_map<std::string, libnvc::object> m_options;

        private:
            std::unique_ptr<board> m_currboard;
            std::unique_ptr<board> m_backboard;

        public:
            nvim_client(libnvc::io_device *, size_t, size_t);

        private:
            void flush_board()
            {
                m_backboard = m_currboard->clone();
            }

        public:
            size_t cursor_x() const
            {
                return m_backboard->cursor_x();
            }

            size_t cursor_y() const
            {
                return m_backboard->cursor_y();
            }

        public:
            size_t width() const
            {
                return m_currboard->width();
            }

            size_t height() const
            {
                return m_currboard->height();
            }

        public:
            void on_option_set(const std::string &option, libnvc::object obj)
            {
                m_options[option] = obj;
            }

        public:
            void on_flush();

        public:
            void on_default_colors_set(int64_t rgb_fg, int64_t rgb_bg, int64_t rgb_sp, int64_t, int64_t)
            {
                hl_attrdef hldef;

                // currently nvim always gives fg, bg, sp as -1
                // -1 means no color set yet, nvim hasn't implement its way to choose a proper default color

                if(rgb_fg == -1){
                    rgb_fg = 0x00000000;
                }

                if(rgb_bg == -1){
                    rgb_bg = 0x00ffffff;
                }

                if(rgb_sp == -1){
                    rgb_sp = 0x00ff0000;
                }

                hldef.color_fg = (rgb_fg & 0x00ffffff);
                hldef.color_bg = (rgb_bg & 0x00ffffff);
                hldef.color_sp = (rgb_sp & 0x00ffffff);

                hldef.color_fg_defined = 1;
                hldef.color_bg_defined = 1;
                hldef.color_sp_defined = 1;

                if(m_hldefs.empty()){
                    m_hldefs.push_back(hldef);
                }else{
                    m_hldefs[0] = hldef;
                }
            }

        public:
            void on_grid_scroll(int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t);

            void on_grid_clear(int64_t)
            {
                m_currboard->clear_char();
            }

            void on_grid_cursor_goto(int64_t, int64_t, int64_t);

        public:
            void on_mode_info_set(bool, const std::vector<libnvc::object> &);
            void on_mode_change(const std::string &, int64_t mode)
            {
                m_mode = (int)(mode);
            }

        public:
            void on_hl_attr_define(int64_t, const std::map<std::string, libnvc::object> &, const std::map<std::string, libnvc::object> &, const std::vector<libnvc::object> &);

        public:
            void on_grid_line(int64_t, int64_t, int64_t, const std::vector<libnvc::object> &);

        private:
            size_t set_cell(size_t, size_t, const std::string &, int64_t, int);

        public:
            const hl_attrdef &get_hl_attrdef(int hl_id) const
            {
                return m_hldefs.at(hl_id);
            }

        public:
            const CELL &get_backcell(size_t x, size_t y) const
            {
                return m_backboard->get_cell(x, y);
            }

        public:
            const auto &get_modedef() const
            {
                return m_modedefs.at(m_mode);
            }
    };

    class nvim_widget
    {
        protected:
            std::unique_ptr<libnvc::nvim_client> m_client;

        private:
            size_t m_cell_width;
            size_t m_cell_height;

        public:
            nvim_widget(
                    libnvc::io_device *,    //
                    size_t,                 // how many cells in a row
                    size_t,                 // how many cells in a column
                    size_t,                 // width  in pixel of one cell
                    size_t);                // height in pixel of one cell

        public:
            size_t width() const
            {
                return m_client->width();
            }

            size_t height() const
            {
                return m_client->height();
            }

        public:
            size_t cell_width() const
            {
                return m_cell_width;
            }

            size_t cell_height() const
            {
                return m_cell_height;
            }

        public:
            virtual void draw_ex(
                    int,    // dst_x
                    int,    // dst_y
                    int,    // src_x
                    int,    // src_y
                    int,    // src_w
                    int);   // src_h

        public:
            virtual void draw_char(
                    const struct libnvc_rect *, // box_draw
                    const struct libnvc_rect *, // box_show
                    const char *,               // len4_cstr
                    uint32_t,                   // foreground 24bits color
                    uint32_t,                   // background 24bits color
                    uint32_t,                   // special    24bits color
                    bool,                       // italic
                    bool,                       // bold
                    bool,                       // underline
                    bool) = 0;                  // undercurl
    };
}