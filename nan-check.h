/**********************************************************************************
* CloudCV Boostrap - A starter template for Node.js with OpenCV bindings.
*                    This project lets you to quickly prototype a REST API
*                    in a Node.js for a image processing service written in C++.
*
* Author: Eugene Khvedchenya <ekhvedchenya@gmail.com>
*
* More information:
*  - https://cloudcv.io
*  - http://computer-vision-talks.com
*
**********************************************************************************/
#pragma once 

#include <nan.h>
#include <node_buffer.h>
#include <functional>
#include <iostream>
#include <sstream>
#include <array>
#include <map>

namespace Nan
{

    class ArgumentMismatchException
    {
    public:

        ArgumentMismatchException(const std::string& msg);
        ArgumentMismatchException(int actual, int expected);
        ArgumentMismatchException(int actual, const std::initializer_list<int>& expected);

        virtual const char * what() const
        {
            return mMessage.c_str();
        }

        virtual ~ArgumentMismatchException()
        {
        }

    private:
        std::string mMessage;
    };

    typedef std::function<bool(Nan::NAN_METHOD_ARGS_TYPE args) > InitFunction;

    class NanMethodArgBinding;
    class NanCheckArguments;

    template <typename EnumType>
    class NanArgStringEnum;

    //////////////////////////////////////////////////////////////////////////

    class NanCheckArguments
    {
    public:
        NanCheckArguments(Nan::NAN_METHOD_ARGS_TYPE args);

        NanCheckArguments& ArgumentsCount(int count);
        NanCheckArguments& ArgumentsCount(int argsCount1, int argsCount2);

        NanMethodArgBinding Argument(int index);

        /**
         * Unwind all fluent calls
         */
        operator bool() const;

        NanCheckArguments& AddAndClause(InitFunction rightCondition);
        NanCheckArguments& Error(std::string * error);

    private:
        Nan::NAN_METHOD_ARGS_TYPE m_args;
        InitFunction          m_init;
        std::string         * m_error;
    };

    //////////////////////////////////////////////////////////////////////////

    template <typename EnumType>
    class NanArgStringEnum
    {
    public:
        explicit NanArgStringEnum(
            std::initializer_list< std::pair<const char*, EnumType> > possibleValues,
            NanMethodArgBinding& owner,
            int argIndex);

        NanCheckArguments& Bind(EnumType& value);
    protected:

        bool TryMatchStringEnum(const std::string& key, EnumType& outValue) const;

    private:
        std::map<std::string, EnumType>     mPossibleValues;
        NanMethodArgBinding&                mOwner;
        int                                 mArgIndex;
    };

    //////////////////////////////////////////////////////////////////////////

    /**
     * @brief This class wraps particular positional argument
     */
    class NanMethodArgBinding
    {
    public:

        template <typename EnumType>
        friend class NanArgStringEnum;

        NanMethodArgBinding(int index, NanCheckArguments& parent);

        NanMethodArgBinding& IsBuffer();
        NanMethodArgBinding& IsFunction();
        NanMethodArgBinding& IsString();
        NanMethodArgBinding& NotNull();
        NanMethodArgBinding& IsArray();
        NanMethodArgBinding& IsObject();

        template <typename T>
        NanArgStringEnum<T> StringEnum(std::initializer_list< std::pair<const char*, T> > possibleValues);

        template <typename T>
        NanCheckArguments& Bind(v8::Local<T>& value);

        template <typename T>
        NanCheckArguments& Bind(T& value);

        template <typename T1, typename T2>
        NanCheckArguments& BindAny(T1& value1, T2& value2);

    private:
        int                 mArgIndex;
        NanCheckArguments&  mParent;
    };

    //////////////////////////////////////////////////////////////////////////

    NanCheckArguments NanCheck(Nan::NAN_METHOD_ARGS_TYPE args);

    //////////////////////////////////////////////////////////////////////////
    // Template functions implementation

    template <typename T>
    NanCheckArguments& NanMethodArgBinding::Bind(v8::Local<T>& value)
    {
        return mParent.AddAndClause([this, &value](Nan::NAN_METHOD_ARGS_TYPE args) {
            value = args[mArgIndex].As<T>();
            return true;
        });
    }


    template <typename T>
    NanCheckArguments& NanMethodArgBinding::Bind(T& value)
    {
        return mParent.AddAndClause([this, &value](Nan::NAN_METHOD_ARGS_TYPE args) {
            try
            {
                value = To<T>(args[mArgIndex]).FromJust();
                return true;
            }
            catch (...)
            {
                return false;
            }
        });
    }

    template <typename T1, typename T2>
    NanCheckArguments& NanMethodArgBinding::BindAny(T1& value1, T2& value2)
    {
        return mParent.AddAndClause([this, &value1, &value2](Nan::NAN_METHOD_ARGS_TYPE args) {
            value1 = To<T1>(args[mArgIndex]).FromJust();
            value2 = To<T2>(args[mArgIndex]).FromJust();
            return true;
        });
    }

    template <typename T>
    NanArgStringEnum<T> NanMethodArgBinding::StringEnum(std::initializer_list< std::pair<const char*, T> > possibleValues)
    {
        return std::move(NanArgStringEnum<T>(possibleValues, IsString(), mArgIndex));
    }

    //////////////////////////////////////////////////////////////////////////

    template <typename T>
    NanArgStringEnum<T>::NanArgStringEnum(
        std::initializer_list< std::pair<const char*, T> > possibleValues,
        NanMethodArgBinding& owner, int argIndex)
        : mPossibleValues(possibleValues.begin(), possibleValues.end())
        , mOwner(owner)
        , mArgIndex(argIndex)
    {
    }

    template <typename T>
    NanCheckArguments& NanArgStringEnum<T>::Bind(T& value)
    {
        return mOwner.mParent.AddAndClause([this, &value](Nan::NAN_METHOD_ARGS_TYPE args) {
            std::string key = To<std::string>(args[mArgIndex]).FromJust();
            return TryMatchStringEnum(key, value);
        });
    }

    template <typename T>
    bool NanArgStringEnum<T>::TryMatchStringEnum(const std::string& key, T& outValue) const
    {
        auto it = mPossibleValues.find(key);
        if (it != mPossibleValues.end())
        {
            outValue = it->second;
            return true;
        }

        //LOG_TRACE_MESSAGE("Cannot map string value " << key << " to any known enum values");
        return false;
    }

}

/**********************************************************************************
* CloudCV Boostrap - A starter template for Node.js with OpenCV bindings.
*                    This project lets you to quickly prototype a REST API
*                    in a Node.js for a image processing service written in C++.
*
* Author: Eugene Khvedchenya <ekhvedchenya@gmail.com>
*
* More information:
*  - https://cloudcv.io
*  - http://computer-vision-talks.com
*
**********************************************************************************/

namespace Nan
{

    ArgumentMismatchException::ArgumentMismatchException(const std::string& msg)
        : mMessage(msg)
    {
    }

    ArgumentMismatchException::ArgumentMismatchException(int actual, int expected)
        : mMessage("Invalid number of arguments passed to a function")
    {
    }

    ArgumentMismatchException::ArgumentMismatchException(int actual, const std::initializer_list<int>& expected)
        : mMessage("Invalid number of arguments passed to a function")
    {
    }

    typedef std::function<bool(Nan::NAN_METHOD_ARGS_TYPE) > InitFunction;

    class NanMethodArgBinding;
    class NanCheckArguments;

    NanMethodArgBinding::NanMethodArgBinding(int index, NanCheckArguments& parent)
        : mArgIndex(index)
        , mParent(parent)
    {
    }

    NanMethodArgBinding& NanMethodArgBinding::IsBuffer()
    {
        auto bind = [this](Nan::NAN_METHOD_ARGS_TYPE args)
        {
            bool isBuf = node::Buffer::HasInstance(args[mArgIndex]);

            if (!isBuf)
                throw ArgumentMismatchException(std::string("Argument ") + std::to_string(mArgIndex) + " violates IsBuffer check");
            return true;
        };

        mParent.AddAndClause(bind);
        return *this;
    }

    NanMethodArgBinding& NanMethodArgBinding::IsFunction()
    {
        auto bind = [this](Nan::NAN_METHOD_ARGS_TYPE args)
        {
            bool isFn = args[mArgIndex]->IsFunction();

            if (!isFn)
                throw ArgumentMismatchException(std::string("Argument ") + std::to_string(mArgIndex) + " violates IsFunction check");

            return true;
        };
        mParent.AddAndClause(bind);
        return *this;
    }

    NanMethodArgBinding& NanMethodArgBinding::IsArray()
    {
        auto bind = [this](Nan::NAN_METHOD_ARGS_TYPE args)
        {
            bool isArr = args[mArgIndex]->IsArray();
            if (!isArr)
                throw ArgumentMismatchException(std::string("Argument ") + std::to_string(mArgIndex) + " violates IsArray check");

            return true;
        };
        mParent.AddAndClause(bind);
        return *this;
    }

    NanMethodArgBinding& NanMethodArgBinding::IsObject()
    {
        auto bind = [this](Nan::NAN_METHOD_ARGS_TYPE args)
        {
            bool isArr = args[mArgIndex]->IsObject();
            if (!isArr)
                throw ArgumentMismatchException(std::string("Argument ") + std::to_string(mArgIndex) + " violates IsObject check");

            return true;
        };
        mParent.AddAndClause(bind);
        return *this;
    }

    NanMethodArgBinding& NanMethodArgBinding::IsString()
    {
        auto bind = [this](Nan::NAN_METHOD_ARGS_TYPE args)
        {
            bool isStr = args[mArgIndex]->IsString() || args[mArgIndex]->IsStringObject();

            if (!isStr)
                throw ArgumentMismatchException(std::string("Argument ") + std::to_string(mArgIndex) + " violates IsString check");

            return true;
        };
        mParent.AddAndClause(bind);
        return *this;
    }

    NanMethodArgBinding& NanMethodArgBinding::NotNull()
    {
        auto bind = [this](Nan::NAN_METHOD_ARGS_TYPE args)
        {
            if (args[mArgIndex]->IsNull())
            {
                throw ArgumentMismatchException(std::string("Argument ") + std::to_string(mArgIndex) + " violates NotNull check");
            }
            return true;
        };
        mParent.AddAndClause(bind);
        return *this;
    }

    NanCheckArguments::NanCheckArguments(Nan::NAN_METHOD_ARGS_TYPE args)
        : m_args(args)
        , m_init([](Nan::NAN_METHOD_ARGS_TYPE args) { return true; })
        , m_error(0)
    {
    }


    NanCheckArguments& NanCheckArguments::ArgumentsCount(int count)
    {
        return AddAndClause([count](Nan::NAN_METHOD_ARGS_TYPE args)
        {
            if (args.Length() != count)
                throw ArgumentMismatchException(args.Length(), count);

            return true;
        });
    }

    NanCheckArguments& NanCheckArguments::ArgumentsCount(int argsCount1, int argsCount2)
    {
        return AddAndClause([argsCount1, argsCount2](Nan::NAN_METHOD_ARGS_TYPE args)
        {
            if (args.Length() != argsCount1 || args.Length() != argsCount2)
                throw ArgumentMismatchException(args.Length(), { argsCount1, argsCount2 });

            return true;
        });
    }

    NanMethodArgBinding NanCheckArguments::Argument(int index)
    {
        return NanMethodArgBinding(index, *this);
    }

    NanCheckArguments& NanCheckArguments::Error(std::string * error)
    {
        m_error = error;
        return *this;
    }

    /**
     * Unwind all fluent calls
     */
    NanCheckArguments::operator bool() const
    {
        try
        {
            return m_init(m_args);
        }
        catch (ArgumentMismatchException& exc)
        {
            if (m_error)
            {
                *m_error = exc.what();
            }
            return false;
        }
        catch (...)
        {
            if (m_error)
            {
                *m_error = "Unknown error";
            }
            return false;
        }
    }

    NanCheckArguments& NanCheckArguments::AddAndClause(InitFunction rightCondition)
    {
        InitFunction prevInit = m_init;
        InitFunction newInit = [prevInit, rightCondition](Nan::NAN_METHOD_ARGS_TYPE args) {
            return prevInit(args) && rightCondition(args);
        };
        m_init = newInit;
        return *this;
    }


    NanCheckArguments Check(Nan::NAN_METHOD_ARGS_TYPE args)
    {
        return std::move(NanCheckArguments(args));
    }
}