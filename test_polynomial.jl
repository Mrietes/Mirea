struct Dual{T} <: Number
    a::T
    b::T
end

real(x::Dual) = x.a
im(x::Dual) = x.b
conj(x::Dual{T}) where T = Dual{T}(x.a, -x.b)

function Base.show(io::IO, p::Dual) #Способ отображения дуального числа в виде строки.
    r = ""
    if p.a != Base. zero(p.a)
        r *= "$(p.a) + "
    end

    if p.b < zero(p.b)
        r = strip(r, [' ', '+'])
        if p.a != Base. zero(p.a)
            r *= " - "
        else
            r *= "-"
        end

        if (p.b != -Base. one(p.b))
            r *= "$(p.b * -1)"
        end
    elseif p.b != Base. zero(p.b) && p.b != Base. one(p.b)
        r *= "$(p.b)"
    end

    if p.b != Base. zero(p.b)
        r *= "ϵ"
    end

    if p.a == Base. zero(p.a) && p.b == Base. zero(p.b)
        r *= "0"
    end

    print(io, strip(r, [' ', '+']))
end

Base. zero(::Dual{T}) where T = Dual{T}(Base. zero(T), Base. zero(T))
Base. one(::Dual{T}) where T = Dual{T}(Base. one(T), Base. zero(T))
Base. eps(::Dual{T}) where T = Dual{T}(Base. zero(T), Base. one(T))

Base. +(x::Dual{T}, y::Dual{T}) where T<:Number = Dual{T}(x.a+y.a, x.b+y.b)
Base. +(x::Dual{T}, y::T) where T<:Number = x + Dual{T}(y, zero(T))
Base. +(x::T, y::Dual{T}) where T<:Number = y + x

Base. -(x::Dual{T}) where T = Dual{T}(-x.a, -x.b)
Base. -(x::Dual{T}, y::Dual{T}) where T<:Number = x + (-y)
Base. -(x::Dual{T}, y::T) where T<:Number = x - Dual{T}(y, zero(T))
Base. -(x::T, y::Dual{T}) where T<:Number = Dual{T}(x, zero(T)) - y

Base. *(x::Dual{T}, y::Dual{T}) where T<:Number = Dual{T}(x.a * y.a, x.a * y.b + x.b * y.a)
Base. *(x::Dual{T}, y::T) where T<:Number = x * Dual{T}(y, zero(T))
Base. *(x::T, y::Dual{T}) where T<:Number = Dual{T}(x, zero(T)) * y

Base. /(x::Dual{T}, y::Dual{T}) where T<:Number = begin
    if y.a == zero(y.a)
        #throw Exception
    end

    return Dual{T}(x.a / y.a, (x.b * y.a - x.a * y.b) / y.a^2)
end
Base. /(x::Dual{T}, y::T) where T<:Number = x / Dual{T}(y, zero(y))
Base. /(x::T, y::Dual{T}) where T<:Number = Dual{T}(x, zero(x)) / y

Base. ^(x::Dual{T}, y::Integer) where T<:Number = Dual{T}(x.a^y, y * x.a^(y-1) * x.b)

Base. ^(x::Dual{T}, y::Dual{T}) where T<:Number = Dual{T}(x.a)
Base. ^(x::Dual{T}, y::T) where T<:Number = Dual{T}(x.a^y, y * x.a^(y-1) * x.b)

Base. sin(x::Dual{T}) where T<:Number = Dual{T}(sin(x.a), x.b * cos(x.a))
Base. cos(x::Dual{T}) where T<:Number = Dual{T}(cos(x.a), x.b * (-sin(x.a)))
Base. tan(x::Dual{T}) where T<:Number = Dual{T}(tan(x.a), x.b * (sec(x.a)^2))
Base. cot(x::Dual{T}) where T<:Number = Dual{T}(cot(x.a), x.b * -(csc(x.a)^2))
Base. asin(x::Dual{T}) where T<:Number = Dual{T}(asin(x.a), x.b * (1 / sqrt(1 - x.a^2)))
Base. acos(x::Dual{T}) where T<:Number = Dual{T}(acos(x.a), x.b * -(1 / sqrt(1 - x.a^2)))
Base. atan(x::Dual{T}) where T<:Number = Dual{T}(atan(x.a), x.b * (1 / (x.a^2 + 1)))
Base. acot(x::Dual{T}) where T<:Number = Dual{T}(acot(x.a), x.b * -(1 / (x.a^2 + 1)))
Base. exp(x::Dual{T}) where T<:Number = Dual{T}(Base. exp(x.a), x.b * Base. exp(x.a))
Base. log(x::Dual{T}) where T<:Number = Dual{T}(log(x.a), x.b / x.a)
Base. sqrt(x::Dual{T}) where T<:Number = begin
    sqrt_a = sqrt(x.a)
    return Dual{T}(sqrt_a, - x.b / (2 * sqrt_a))
end

valdiff(f::Function, x) = begin #Позволяет вычислять значение и производную функции в точке x
    dual_x = Dual(x, one(x))
    dual_f = f(dual_x)
    return (dual_f.a, dual_f.b)
end

valdiff(p::Polynomial, x, ::Type{Dual}) = begin #Вычисляет значение и произвордную многочлена в точке  x
    len = length(p.coeffs)
    sum_1 = zero(x)
    sum_2 = zero(x)
    for i = 1:len
        pow = len - p.coeffs[i]
        println("$(p.coeffs[i])x^$pow")
        val = valdiff((x -> p.coeffs[i] * (pow != 0 ? x^pow : x/x)), x)
        sum_1 += val[1]
        sum_2 += val[2]
    end

    return (sum_1, sum_2)
end

d1 = Dual{Int64}(0, -1)
d2 = Dual{Int64}(-6, -2)
println(Base. one(d1))
println("($d1) + ($d2) = $(d1 + d2)")
println("($d1) + (2) = $(d1 + 2)")
println("(2) - ($d1) = $(2 - d1)")
println("($d1) * ($d2) = $(d1 * d2)")
println("(2) * ($d1) * (-3) = $(2 * d1 * -3)")
println("conj ($d2) = $(conj(d2))")
println("($d2) * $(conj(d2)) = $(d2 * conj(d2))")

