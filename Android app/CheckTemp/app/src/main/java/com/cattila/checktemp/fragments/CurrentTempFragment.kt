package com.cattila.checktemp.fragments

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.databinding.DataBindingUtil
import androidx.fragment.app.Fragment
import com.android.volley.Request
import com.android.volley.Response
import com.android.volley.toolbox.StringRequest
import com.android.volley.toolbox.Volley
import com.cattila.checktemp.R
import com.cattila.checktemp.databinding.FragmentCurrentTempBinding

class CurrentTempFragment : Fragment() {

    lateinit var binding: FragmentCurrentTempBinding

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        binding = DataBindingUtil.inflate(
            inflater, R.layout.fragment_current_temp, container, false
        )

        val textView = binding.textView

        val queue = Volley.newRequestQueue(this.context)
        val url = "http://192.168.1.109/gateway.php?range=current"

        val stringRequest = StringRequest(
            Request.Method.GET, url,
            Response.Listener<String> {response ->
                textView.text = response
            },
            Response.ErrorListener { textView.text = "That didn't work!" })

        queue.add(stringRequest)

        return binding.root
    }
}
