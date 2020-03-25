package com.cattila.checktemp.fragments

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.databinding.DataBindingUtil
import com.android.volley.Request
import com.android.volley.Response
import com.android.volley.toolbox.StringRequest
import com.android.volley.toolbox.Volley
import com.cattila.checktemp.R
import com.cattila.checktemp.databinding.FragmentLastWeekTempBinding

class LastWeekTempFragment : Fragment() {

    lateinit var binding: FragmentLastWeekTempBinding

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        binding = DataBindingUtil.inflate(
            inflater, R.layout.fragment_last_week_temp, container, false
        )

        val textView = binding.textView

        val queue = Volley.newRequestQueue(this.context)
        val url = "http://192.168.1.109/gateway.php?range=1week"

        val stringRequest = StringRequest(
            Request.Method.GET, url,
            Response.Listener<String> { response ->
                textView.text = response
            },
            Response.ErrorListener { textView.text = "That didn't work!" })

        queue.add(stringRequest)

        return binding.root
    }
}
